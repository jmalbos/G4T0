#ifndef STEPPING_ACTION_HH
#define STEPPING_ACTION_HH

#include "G4UserSteppingAction.hh"
#include "globals.hh"
#include <fstream>

class G4LogicalVolume;

class EventAction;

/// Stepping action class

class SteppingAction : public G4UserSteppingAction
{
public:
  SteppingAction();
  ~SteppingAction();

  // method from the base class
  void UserSteppingAction(const G4Step *) override;

private:
  std::ofstream output_file_;
};

#endif
