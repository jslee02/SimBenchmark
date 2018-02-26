//
// Created by kangd on 15.02.18.
//

#include "odeSim/World_RG.hpp"

#include "rolling.hpp"

int main(int argc, char* argv[]) {

  double dt = benchmark::dt;

  ode_sim::SolverOption solverOption = ode_sim::SOLVER_STANDARD;
  std::string solverName = "std";

  if (argc == 2) {
    dt = atof(argv[1]);
    RAIINFO("----------------------")
    RAIINFO("ODESim")
    RAIINFO("timestep = " << dt);
  } else if (argc == 3) {
    dt = atof(argv[1]);

    if(strcmp(argv[2],"std")==0) {
      solverOption = ode_sim::SOLVER_STANDARD;
      solverName = "std";
    } else if(strcmp(argv[2],"quick")==0) {
      solverOption = ode_sim::SOLVER_QUICK;
      solverName = "quick";
    }

    RAIINFO("----------------------")
    RAIINFO("ODESim")
    RAIINFO("timestep = " << dt);
    RAIINFO("solver   = " << solverName);
  }

  // logger
  std::string path = benchmark::dataPath + benchmark::parentDir + "ode/" + solverName;
  std::string name = std::to_string(dt);
  rai::Utils::logger->setLogPath(path);
  rai::Utils::logger->setLogFileName(name);
  rai::Utils::logger->setOptions(rai::Utils::ONEFILE_FOR_ONEDATA);
  rai::Utils::logger->addVariableToLog(3, "velbox", "linear velocity of box");
  rai::Utils::logger->addVariableToLog(3, "velball", "linear velocity of ball");
  rai::Utils::logger->addVariableToLog(3, "posbox", "position of box");
  rai::Utils::logger->addVariableToLog(3, "posball", "position of ball");

  // timer
  std::string timer = name + "timer";
  rai::Utils::timer->setLogPath(path);
  rai::Utils::timer->setLogFileName(timer);

  // sim
  ode_sim::World_RG *sim;

  if(benchmark::visualize)
    sim = new ode_sim::World_RG(800, 600, 0.5, benchmark::NO_BACKGROUND, solverOption);
  else
    sim = new ode_sim::World_RG(solverOption);

  sim->setGravity(benchmark::gravity);
  sim->setERP(benchmark::erp, 0, 0);

  // add objects
  auto checkerboard = sim->addCheckerboard(5.0, 100.0, 100.0, 0.1);
  checkerboard->setFrictionCoefficient(benchmark::groundMu);

  auto box = sim->addBox(20, 20, 1, 10);
  box->setPosition(0, 0, 0.5 - benchmark::initPenetration);
  box->setFrictionCoefficient(benchmark::boxMu);

  std::vector<benchmark::SingleBodyHandle> objectList;

  for(int i = 0; i < 5; i++) {
    for(int j = 0; j < 5; j++) {
      auto ball = sim->addSphere(0.5, 1);
      ball->setPosition(i * 2.0 - 4.0, j * 2.0 - 4.0, 1.5 - benchmark::initPenetration * 2);
      ball->setFrictionCoefficient(benchmark::ballMu);
      objectList.push_back(ball);
    }
  }

  // simulation loop
  // press 'q' key to quit
  rai::Utils::timer->startTimer("rolling");
  if(benchmark::visualize) {
    // camera relative position
    sim->cameraFollowObject(checkerboard, {30, 0, 10});
    sim->setLightPosition(benchmark::lightX, benchmark::lightY, benchmark::lightZ);

    for(int i = 0; i < benchmark::simulationTime / dt && sim->visualizerLoop(dt); i++) {
      box->setExternalForce(benchmark::force);
      // log
      rai::Utils::logger->appendData("velbox", box->getLinearVelocity().data());
      rai::Utils::logger->appendData("velball", objectList[0]->getLinearVelocity().data());
      rai::Utils::logger->appendData("posbox", box->getPosition().data());
      rai::Utils::logger->appendData("posball", objectList[0]->getPosition().data());
      sim->integrate(dt);
    }
  }
  else {
    for(int i = 0; i < benchmark::simulationTime / dt; i++) {
      box->setExternalForce(benchmark::force);
      // log
      rai::Utils::logger->appendData("velbox", box->getLinearVelocity().data());
      rai::Utils::logger->appendData("velball", objectList[0]->getLinearVelocity().data());
      rai::Utils::logger->appendData("posbox", box->getPosition().data());
      rai::Utils::logger->appendData("posball", objectList[0]->getPosition().data());
      sim->integrate(dt);
    }
  }
  rai::Utils::timer->stopTimer("rolling");

  // delete sim
  delete sim;

  // time log
  rai::Utils::timer->dumpToStdOuput();

  return 0;
}