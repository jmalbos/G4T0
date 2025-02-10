#ifndef PHYSICS_LIST_HH
#define PHYSICS_LIST_HH

#include "G4VModularPhysicsList.hh"

/// Modular physics list
///
/// It includes the folowing physics builders
/// - G4DecayPhysics
/// - G4RadioactiveDecayPhysics
/// - G4EmStandardPhysics

class PhysicsList : public G4VModularPhysicsList
{
public:
  PhysicsList();
  ~PhysicsList() override = default;

  void SetCuts() override;
};

#endif
