//
// Created by jhwangbo on 04/12/17.
//
#include <BtSim.hpp>
#include <string>
#include "raiCommon/utils/StopWatch.hpp"

int main() {

  std::string urdfPath(__FILE__);
  while (urdfPath.back() != '/')
    urdfPath.erase(urdfPath.size() - 1, 1);
  urdfPath += "../../../res/Atlas/";

  bullet_sim::BtSim sim(800, 600, 0.2, bullet_sim::SOLVER_MULTI_BODY, benchmark::NO_BACKGROUND);
  sim.setLightPosition(0, -10, 10);

  auto atlas = sim.addArticulatedSystem(urdfPath);
  auto checkerBoard = sim.addCheckerboard(2, 100, 100, 0.1, bo::BOX_SHAPE, 1, -1, bo::GRID);

  Eigen::VectorXd gc(atlas->getStateDimension());
  Eigen::VectorXd gv(atlas->getDOF()), tau(atlas->getDOF());

  std::cout << "dof "<<atlas->getDOF()<<"\n";

  gc.setZero();
  gc.segment<7>(0) << 0,0,3,1,0,0,0;
  gv.setZero();
  tau.setZero();

  atlas->setState(gc, gv);

  const double dt = 1.0 / 5000.0;
  int counter = 0;
  sim.cameraFollowObject(checkerBoard, {5, 0, 0});

  sim.loop(dt);

  std::cout<<"contact problem size: "<< sim.getWorldNumContacts() <<"\n";

//  StopWatch watch;
//  watch.start();
//  for(int i=0; i<10000; i++)
//    sim.integrate(dt);
//  std::cout<<"Number of steps in one second  "<<10.0/watch.measure()<<"k\n";
}