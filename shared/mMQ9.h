//
// You received this file as part of Finroc
// A framework for intelligent robot control
//
// Copyright (C) Patrick Wolf
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
//----------------------------------------------------------------------
/*!\file    projects/smart_home/heat_control/mMQ9.h
 *
 * \author  Patrick Wolf
 *
 * \date    2015-03-12
 *
 * \brief Contains mMQ9
 *
 * \b mMQ9
 *
 * MQ gas sensor module
 */
//----------------------------------------------------------------------
#ifndef __projects__smart_home__shared__mMQ9_h__
#define __projects__smart_home__shared__mMQ9_h__

#include "plugins/structure/tModule.h"

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Namespace declaration
//----------------------------------------------------------------------
namespace finroc
{
namespace smart_home
{
namespace shared
{

//----------------------------------------------------------------------
// Forward declarations / typedefs / enums
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Class declaration
//----------------------------------------------------------------------
//! SHORT_DESCRIPTION
/*!
 * module that provides a hardware interface, a/d conversion and is capable of setting pumps.
 */
class mMQ9: public structure::tModule
{

//----------------------------------------------------------------------
// Ports (These are the only variables that may be declared public)
//----------------------------------------------------------------------
public:

  tInput<rrlib::si_units::tVoltage<double>> in_voltage;

  tOutput<rrlib::si_units::tAmountOfSubstance<double>> out_carbon_monoxid;
  tOutput<rrlib::si_units::tElectricResistance<double>> out_resistance;

  tParameter<rrlib::si_units::tElectricResistance<double>> par_pre_resistance;
  tParameter<rrlib::si_units::tVoltage<double>> par_reference_voltage;
  tParameter<rrlib::si_units::tVoltage<double>> par_supply_voltage;

//----------------------------------------------------------------------
// Public methods and typedefs
//----------------------------------------------------------------------
public:

  mMQ9(core::tFrameworkElement *parent, const std::string &name = "MQ9") :
    tModule(parent, name),
    par_pre_resistance(31.0),
    par_reference_voltage(5.0),
    par_supply_voltage(5.0)
  {}

//----------------------------------------------------------------------
// Protected methods
//----------------------------------------------------------------------
protected:

  /*! Destructor
   *
   * The destructor of modules is declared protected to avoid accidental deletion. Deleting
   * modules is already handled by the framework.
   */
  ~mMQ9() {}

  /*!
   * Determines resistance value of PT sensor
   * @param voltage A/D voltage
   * @param reference_voltage reference voltage of A/D converter
   * @param pre_resistance pre resistance of PT sensor
   * @return resistance
   */
  inline rrlib::si_units::tElectricResistance<double> GetResistance(
    const rrlib::si_units::tVoltage<double> & voltage,
    const rrlib::si_units::tVoltage<double> & reference_voltage,
    const rrlib::si_units::tElectricResistance<double> & pre_resistance) const
  {
    if (voltage.Value() == 0)
    {
      return pre_resistance;
    }

    return pre_resistance * ((reference_voltage / voltage).Value() - 1.0);
  }

//----------------------------------------------------------------------
// Private fields and methods
//----------------------------------------------------------------------
private:

  inline virtual void Update() override
  {
    auto resistance =  GetResistance(in_voltage.Get(), par_reference_voltage.Get(), par_pre_resistance.Get());

    out_resistance.Publish(resistance, in_voltage.GetTimestamp());
  }
};

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}
}

#endif
