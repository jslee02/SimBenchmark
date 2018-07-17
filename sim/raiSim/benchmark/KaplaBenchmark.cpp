//
// Created by kangd on 09.05.18.
//

#include <raiSim/World_RG.hpp>

#include "KaplaBenchmark.hpp"

rai_sim::World_RG *sim;
std::vector<rai_sim::SingleBodyHandle> objList;
po::options_description desc;

void setupSimulation() {
  if (benchmark::building::options.gui)
    sim = new rai_sim::World_RG(800, 600, 0.015, rai_sim::NO_BACKGROUND);
  else
    sim = new rai_sim::World_RG();

  // erp
  if(benchmark::building::options.erpYN)
    sim->setERP(benchmark::building::params.erp);
  else
    sim->setERP(0);

  // time step
  sim->setTimeStep(benchmark::building::params.dt);

  sim->setContactSolverParam(1.0, 0.7, 1.0,
                             benchmark::building::options.numSolverIter,
                             1e-12);
}

void setupWorld() {
  // add objects
  auto checkerboard = sim->addCheckerboard(2.0, 400.0, 400.0, 0.1, 1, -1);
  std::vector<float> spec = {0.0, 0.0, 0.0}, amb = {5.0, 5.0, 5.0}, diff = {0.0,0.0,0.0};
  checkerboard.visual()[0]->setLightProp(amb, diff, spec, 0.2);

  // block size
  const float shortLen = benchmark::building::params.shortLen;
  const float longLen = benchmark::building::params.longLen;
  const float heightLen = benchmark::building::params.heightLen;

  // num of blocks
  // numFloor x numBase + numFloor x (numWall x 2 + 1)
  const int numFloor = benchmark::building::params.numFloor;
  const int numBase = benchmark::building::params.numBase;
  const int numWall = numBase / 2;

  for(int i = 0; i < numFloor; i++) {
    // i floor
    for(int j = 0; j < numBase; j++) {
      // base
      auto base = sim->addBox(shortLen, longLen + 0.05, heightLen, 10.0);
      base->setPosition(j * longLen, 0, i * heightLen * 2 + 0.05);
      objList.push_back(base);

      if(benchmark::building::options.gui)
        base.visual()[0]->setColor({0.0, 0.0, 1.0});
    }

    for(int j = 0; j < numWall; j++) {
      // right wall
      auto wall = sim->addBox(longLen, shortLen, heightLen, 10.0);
      wall->setPosition(j * longLen * 2 + 0.1, -0.5 * longLen, i * heightLen * 2 + 0.15);
      objList.push_back(wall);

      if(benchmark::building::options.gui)
        wall.visual()[0]->setColor({0.0, 1.0, 0.0});
    }

    for(int j = 0; j < numWall - 1; j++) {
      // left wall
      auto wall = sim->addBox(longLen, shortLen, heightLen, 10.0);
      wall->setPosition(j * longLen * 2 + 0.3, 0.5 * longLen, i * heightLen * 2 + 0.15);
      objList.push_back(wall);

      if(benchmark::building::options.gui)
        wall.visual()[0]->setColor({1.0, 0.0, 0.0});
    }

    // first wall on left
    auto wall1 = sim->addBox(longLen, shortLen, heightLen, 10.0);
    wall1->setPosition(0.1, 0.5 * longLen, i * heightLen * 2 + 0.15);
    objList.push_back(wall1);

    // last wall on left
    auto wall2 = sim->addBox(longLen, shortLen, heightLen, 10.0);
    wall2->setPosition((numWall - 1) * longLen * 2 + 0.1, 0.5 * longLen, i * heightLen * 2 + 0.15);
    objList.push_back(wall2);

    if(benchmark::building::options.gui) {
      wall1.visual()[0]->setColor({1.0, 0.0, 0.0});
      wall2.visual()[0]->setColor({1.0, 0.0, 0.0});
    }
  }

  // gravity
  sim->setGravity({0, 0, benchmark::building::params.g});

  if(benchmark::building::options.gui) {
    sim->setLightPosition((float)benchmark::building::params.lightPosition[0],
                          (float)benchmark::building::params.lightPosition[1],
                          (float)benchmark::building::params.lightPosition[2]);
    sim->cameraFollowObject(objList[47], {0, 10, 0});
  }
}

benchmark::building::Data simulationLoop() {
  if(benchmark::building::options.saveVideo)
    sim->startRecordingVideo("/tmp", "rai-building");

  // data
  benchmark::building::Data data;
  data.setN(unsigned(benchmark::building::params.T / benchmark::building::params.dt));

  // timer start
  StopWatch watch;
  watch.start();

  int i;
  for(i = 0; i < (int) (benchmark::building::params.T / benchmark::building::params.dt); i++) {
    // gui
    if (benchmark::building::options.gui && !sim->visualizerLoop())
      break;

    // num contacts
    data.numContacts.push_back(sim->getContactProblem().size());

    if(benchmark::building::options.collapse && objList.back()->getPosition()[2] <
        benchmark::building::params.heightLen * (benchmark::building::params.numFloor - 1) * 2) {
      // break if the building collapses
      RAIINFO("building collapsed after " << i << " steps = " << i * benchmark::building::params.dt << " sec!")
      break;
    }

    sim->integrate();
  }

  if(benchmark::building::options.saveVideo)
    sim->stopRecordingVideo();

  data.time = watch.measure();
  data.step = i;
  return data;
}

int main(int argc, const char* argv[]) {

  benchmark::building::addDescToOption(desc);
  benchmark::building::getOptionsFromArg(argc, argv, desc);
  benchmark::building::getParamsFromYAML(benchmark::building::getYamlpath().c_str(),
                                         benchmark::RAI);

  setupSimulation();
  setupWorld();

  RAIINFO(
      std::endl << "=======================" << std::endl
                << "Simulator : DART" << std::endl
                << "GUI       : " << benchmark::building::options.gui << std::endl
                << "ERP       : " << benchmark::building::options.erpYN << std::endl
                << "Num iter  : " << benchmark::building::options.numSolverIter << std::endl
                << "Tolerance : " << benchmark::building::options.solverTol << std::endl
                << "Timestep  : " << benchmark::building::params.dt << std::endl
                << "Num block : " << objList.size() << std::endl
                << "-----------------------"
  )

  benchmark::building::Data data = simulationLoop();

  if(benchmark::building::options.csv)
    benchmark::building::printCSV(benchmark::building::getCSVpath(),
                                  "RAI",
                                  "RAI",
                                  "RAI",
                                  "RAI",
                                  data.time,
                                  data.step,
                                  data.computeMeanContacts());

  RAIINFO(
      std::endl << "Avg. Num Contacts : " << data.computeMeanContacts() << std::endl
                << "CPU time          : " << data.time << std::endl
                << "num steps         : " << data.step << std::endl
                << "speed             : " << data.step / data.time << std::endl
                << "=======================" << std::endl
  )

  delete sim;
  return 0;
}
