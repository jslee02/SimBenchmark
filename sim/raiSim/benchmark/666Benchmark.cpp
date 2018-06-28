//
// Created by kangd on 15.02.18.
//

#include <raiSim/World_RG.hpp>

#include "666Benchmark.hpp"

rai_sim::World_RG *sim;
std::vector<rai_sim::SingleBodyHandle> objList;
po::options_description desc;

void setupSimulation() {
  if (benchmark::sixsixsix::options.gui)
    sim = new rai_sim::World_RG(800, 600, 1, rai_sim::NO_BACKGROUND);
  else
    sim = new rai_sim::World_RG();

  // erp
  if(benchmark::sixsixsix::options.erpYN)
    sim->setERP(benchmark::sixsixsix::params.erp);
  else
    sim->setERP(0);

  // time step
  sim->setTimeStep(benchmark::sixsixsix::options.dt);
}

double penetrationCheck() {
  double error = 0;
  int numObj = objList.size();

  for (int i = 0; i < numObj; i++) {
    for (int j = i + 1; j < numObj; j++) {
      double dist = (objList[i]->getPosition() - objList[j]->getPosition()).norm();

      // error between spheres
      if (dist < benchmark::sixsixsix::params.ballR * 2)
        error += (benchmark::sixsixsix::params.ballR * 2 - dist) * (benchmark::sixsixsix::params.ballR * 2 - dist);
    }

    // error sphere ~ ground
    if (objList[i]->getPosition()[2] < benchmark::sixsixsix::params.ballR) {
      error +=
          pow(benchmark::sixsixsix::params.ballR - objList[i]->getPosition()[2], 2);
    }
  }

  return error;
}

double computeEnergy() {
  double energy = 0;
  for(int j = 0; j < objList.size(); j++)
    energy += objList[j]->getEnergy({0, 0, benchmark::sixsixsix::params.g});
  return energy;
};

void setupWorld() {

  // gravity
  sim->setGravity({0, 0, benchmark::sixsixsix::params.g});

  // materials
  rai_sim::MaterialManager materials;
  materials.setMaterialNames({"ground", "ball"});
  if(benchmark::sixsixsix::options.elasticCollision) {
    materials.setMaterialPairProp("ground", "ball",
                                  0, 1, 0);
    materials.setMaterialPairProp("ball", "ball",
                                  0, 1, 0);
  }
  else {
    materials.setMaterialPairProp("ground", "ball",
                                  0, 0, 0.01);
    materials.setMaterialPairProp("ball", "ball",
                                  0, 0, 0.01);
  }
  sim->updateMaterialProp(materials);

  // random number generator
  rai::RandomNumberGenerator<double> rand;
  rand.seed(benchmark::sixsixsix::params.randomSeed);

  auto checkerboard = sim->addCheckerboard(1.0, 500.0, 500.0, 0.1, 1, -1);
  std::vector<float> spec = {0.0, 0.0, 0.0}, amb = {5.0, 5.0, 5.0}, diff = {0.0,0.0,0.0};
  checkerboard.visual()[0]->setLightProp(amb, diff, spec, 0.2);
  checkerboard->setMaterial(sim->getMaterialKey("ground"));

  for(int i = 0; i < benchmark::sixsixsix::params.n; i++) {
    for(int j = 0; j < benchmark::sixsixsix::params.n; j++) {
      for(int k = 0; k < benchmark::sixsixsix::params.n; k++) {

        // add object
        auto obj = sim->addSphere(benchmark::sixsixsix::params.ballR,
                                  benchmark::sixsixsix::params.ballM);

        // set position
        double x =
            double(i) * benchmark::sixsixsix::params.gap
                + rand.sampleUniform01() * benchmark::sixsixsix::params.perturbation;
        double y =
            double(j) * benchmark::sixsixsix::params.gap
                + rand.sampleUniform01() * benchmark::sixsixsix::params.perturbation;
        double z =
            double(k) * benchmark::sixsixsix::params.gap
                + rand.sampleUniform01() * benchmark::sixsixsix::params.perturbation
                + benchmark::sixsixsix::params.H;

        obj->setPosition(x, y, z);
        obj->setMaterial(sim->getMaterialKey("ball"));
//        obj->setOrientationRandom();

        if(benchmark::sixsixsix::options.gui) {
          if((i + j + k) % 3 == 0) {
            obj.visual()[0]->setColor({1,
                                       0,
                                       0});
          }
          else if((i + j + k) % 3 == 1) {
            obj.visual()[0]->setColor({0,
                                       1,
                                       0});
          }
          else if((i + j + k) % 3 == 2) {
            obj.visual()[0]->setColor({0,
                                       0,
                                       1});
          }
        }

        objList.push_back(obj);
      }
    }
  }

  if(benchmark::sixsixsix::options.gui) {
    sim->setLightPosition((float)benchmark::sixsixsix::params.lightPosition[0],
                          (float)benchmark::sixsixsix::params.lightPosition[1],
                          (float)benchmark::sixsixsix::params.lightPosition[2]);
    sim->cameraFollowObject(objList[objList.size() / 2], {0, 5, 1});
  }
}

double simulationLoop(bool timer = true, bool error = true) {
  // gui
  if(benchmark::sixsixsix::options.gui && benchmark::sixsixsix::options.saveVideo)
    sim->startRecordingVideo("/tmp", "rai-666");

  // resever error vector
  benchmark::sixsixsix::data.setN(unsigned(benchmark::sixsixsix::options.T / benchmark::sixsixsix::options.dt));

  // timer start
  StopWatch watch;
  if(timer)
    watch.start();

  for(int i = 0; i < (int) (benchmark::sixsixsix::options.T / benchmark::sixsixsix::options.dt); i++) {
    if (benchmark::sixsixsix::options.gui && !sim->visualizerLoop())
      break;

    // data save
    if (error) {
      static double E0 = 0;
      if(i==0)
        E0 = computeEnergy();

      if (benchmark::sixsixsix::options.elasticCollision) {
        double error = pow(computeEnergy() - E0, 2);
        benchmark::sixsixsix::data.error.push_back(error);
      }
      else {
        double error = penetrationCheck();
        benchmark::sixsixsix::data.error.push_back(error);
      }
    }

    sim->integrate();
  }

  if(benchmark::sixsixsix::options.saveVideo)
    sim->stopRecordingVideo();

  double time = 0;
  if(timer)
    time = watch.measure();
  return time;
}

int main(int argc, const char* argv[]) {

  benchmark::sixsixsix::addDescToOption(desc);
  benchmark::sixsixsix::getOptionsFromArg(argc, argv, desc);
  benchmark::sixsixsix::getParamsFromYAML(benchmark::sixsixsix::getYamlpath().c_str(),
                                         benchmark::RAI);

  RAIINFO(
      std::endl << "=======================" << std::endl
                << "Simulator: " << "RAI" << std::endl
                << "GUI      : " << benchmark::sixsixsix::options.gui << std::endl
                << "ERP      : " << benchmark::sixsixsix::options.erpYN << std::endl
                << "Elastic  : " << benchmark::sixsixsix::options.elasticCollision << std::endl
                << "Timestep : " << benchmark::sixsixsix::options.dt << std::endl
                << "Solver   : " << "RAI" << std::endl
                << "-----------------------"
  )

  // trial1: get Error
  setupSimulation();
  setupWorld();
  simulationLoop(false, true);
  double error = benchmark::sixsixsix::data.computeError();

  // reset
  objList.clear();
  delete sim;

  // trial2: get CPU time
  setupSimulation();
  setupWorld();
  double time = simulationLoop(true, false);


  if(benchmark::sixsixsix::options.csv)
    benchmark::sixsixsix::printCSV(benchmark::sixsixsix::getCSVpath(),
                                   "RAI",
                                   "RAI",
                                   "RAI",
                                   "RAI",
                                   time,
                                   error);

  RAIINFO(
      std::endl << "CPU time   : " << time << std::endl
                << "mean error : " << error << std::endl
                << "=======================" << std::endl
  )

  delete sim;
  return 0;

}
