#include "DetectorConstruction.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"


G4VPhysicalVolume *DetectorConstruction::Construct()
{
  // Get nist material manager
  G4NistManager *nist = G4NistManager::Instance();

  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;
  //

  // ------------- Volumes --------------
  //
  // World
  //
  G4double world_hx = 500. * mm;
  G4double world_hy = 500. * mm;
  G4double world_hz = 500. * mm;
  G4Material *world_mat = nist->FindOrBuildMaterial("G4_AIR");

  auto solidWorld = new G4Box("World",                       // its name
                              world_hx, world_hy, world_hz); // its size

  auto logicWorld = new G4LogicalVolume(solidWorld, // its solid
                                        world_mat,  // its material
                                        "World");   // its name

  auto physWorld = new G4PVPlacement(nullptr,         // no rotation
                                     G4ThreeVector(), // at (0,0,0)
                                     logicWorld,      // its logical volume
                                     "World",         // its name
                                     nullptr,         // its mother  volume
                                     false,           // no boolean operation
                                     0,               // copy number
                                     checkOverlaps);  // overlaps checking

  //
  // Argon gas
  //
  G4double gas_radius = 50. * mm;
  G4double gas_length = 100. * mm;
  G4Material *gas_mat = nist->FindOrBuildMaterial("G4_Ar");

  auto gas_solid = new G4Tubs("Gas",           // name
                              0.,              // inner radius
                              gas_radius,      // outer radius
                              gas_length / 2., // half length
                              0.,              // start phi angle
                              360. * deg);     // final phi angle

  auto gas_logic = new G4LogicalVolume(gas_solid, gas_mat, "Gas");

  new G4PVPlacement(nullptr,         // no rotation
                    G4ThreeVector(), // at position
                    gas_logic,       // its logical volume
                    "Gas",           // its name
                    logicWorld,      // its mother  volume
                    false,           // no boolean operation
                    0,               // copy number
                    checkOverlaps);  // overlaps checking

  //
  // Photon detector
  //
  G4double phdet_hx = 10. * mm;
  G4double phdet_hy = 10. * mm;
  G4double phdet_hz = 1. * mm;
  G4Material *phdet_mat = nist->FindOrBuildMaterial("G4_Si");

  auto phdet_solid = new G4Box("PhDet",                       // its name
                               phdet_hx, phdet_hy, phdet_hz); // its size

  auto phdet_logic = new G4LogicalVolume(phdet_solid, // its solid
                                         phdet_mat,   // its material
                                         "PhDet");    // its name

  new G4PVPlacement(nullptr,         // no rotation
                    G4ThreeVector(), // at (0,0,0)
                    phdet_logic,     // its logical volume
                    "PhDet",         // its name
                    gas_logic,       // its mother  volume
                    false,           // no boolean operation
                    0,               // copy number
                    checkOverlaps);  // overlaps checking

  return physWorld;
}
