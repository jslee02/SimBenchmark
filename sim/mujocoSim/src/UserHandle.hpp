//
// Created by jhwangbo on 02/02/18.
//

#ifndef BULLETSIM_USERHANDLE_HPP
#define BULLETSIM_USERHANDLE_HPP

#include <raiGraphics/RAI_graphics.hpp>
#include <common/UserHandle.hpp>

#include "object/MjcSingleBodyObject.hpp"
//#include "object/ArticulatedSystem/ArticulatedSystem.hpp"

namespace mujoco_sim {

typedef benchmark::UserObjectHandle<mujoco_sim::object::MjcSingleBodyObject> SingleBodyHandle;
//typedef benchmark::UserObjectHandle<mujoco_sim::object::ArticulatedSystem> ArticulatedSystemHandle;
//typedef UserObjectHandle<bullet_sim::object::Compound> CompoundHandle;
//typedef UserWireHandle<bullet_sim::Wire> WireHandle;
//typedef UserWireHandle<bullet_sim::StiffWire> StiffWireHandle;
//typedef UserWireHandle<bullet_sim::CompliantWire> CompliantWireHandle;

} // benchmark

#endif //BENCHMARK_USERHANDLE_HPP
