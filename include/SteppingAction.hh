#ifndef STEPPING_ACTION_HH
#define STEPPING_ACTION_HH

#include "G4UserSteppingAction.hh"
#include "globals.hh"
#include <map>

class G4LogicalVolume;

class EventAction;

/// Stepping action class

class SteppingAction : public G4UserSteppingAction
{
public:
  SteppingAction();
  ~SteppingAction() override = default;

  // method from the base class
  void UserSteppingAction(const G4Step *) override;
};

#endif
