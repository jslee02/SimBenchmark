//
// Created by kangd on 10.02.18.
//

#ifndef BULLETSIM_OBJECT_HPP
#define BULLETSIM_OBJECT_HPP

#include <btBulletCollisionCommon.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <Eigen/Geometry>
#include <raiSim/math.hpp>

namespace bullet_sim {
namespace object {

class SingleBodyObject {

 public:
  SingleBodyObject(double mass);
  virtual ~SingleBodyObject();

  btRigidBody *getRigidBody_() const;

  virtual const Eigen::Map<Eigen::Matrix<double, 4, 1>> getQuaternion();
  virtual void getQuaternion(rai_sim::Vec<4>& quat);
  virtual const Eigen::Map<Eigen::Matrix<double, 3, 3> > getRotationMatrix();
  virtual void getRotationMatrix(rai_sim::Mat<3,3>& rotation);
  virtual const Eigen::Map<Eigen::Matrix<double, 3, 1> > getPosition();
  virtual const Eigen::Map<Eigen::Matrix<double, 3, 1> > getComPosition();
  virtual  const Eigen::Map<Eigen::Matrix<double, 3, 1> > getLinearVelocity();
  virtual const Eigen::Map<Eigen::Matrix<double, 3, 1> > getAngularVelocity();
  virtual void getPosition_W(rai_sim::Vec<3>& pos_w);

  virtual void setPosition(Eigen::Vector3d originPosition);
  virtual void setPosition(double x, double y, double z);
  virtual void setOrientation(Eigen::Quaterniond quaternion);
  virtual void setOrientation(double w, double x, double y, double z);
  virtual void setOrientation(Eigen::Matrix3d rotationMatrix);
//  void setOrientationRandom();
  virtual void setPose(Eigen::Vector3d originPosition, Eigen::Quaterniond quaternion);
  virtual void setPose(Eigen::Vector3d originPosition, Eigen::Matrix3d rotationMatrix);
  virtual void setVelocity(Eigen::Vector3d linearVelocity, Eigen::Vector3d angularVelocity);
  virtual void setVelocity(double dx, double dy, double dz, double wx, double wy, double wz);

  virtual bool isVisualizeFramesAndCom() const;

 protected:
  double mass_;

  btCollisionShape *collisionShape_;
  btRigidBody *rigidBody_;
  btMotionState *motionState_;

  // from object
  bool visualizeFramesAndCom_ = true;

};

} // object
} // bullet_sim

#endif // BULLETSIM_OBJECT_HPP
