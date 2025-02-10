#include "DetectorConstruction.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"

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
  G4double gas_diam = 250. * mm;
  G4double gas_length = 270. * mm;
  G4Material *gas_mat = nist->FindOrBuildMaterial("G4_Ar");
  G4ThreeVector gas_pos = G4ThreeVector();

  auto gas_solid = new G4Tubs("Gas",           // name
                              0.,              // inner radius
                              gas_diam / 2.,   // outer radius
                              gas_length / 2., // length
                              0.,              // start phi angle
                              360. * deg);     // final phi angle

  auto gas_logic = new G4LogicalVolume(gas_solid, gas_mat, "Gas");

  new G4PVPlacement(nullptr,        // no rotation
                    gas_pos,        // at position
                    gas_logic,      // its logical volume
                    "Gas",          // its name
                    logicWorld,     // its mother  volume
                    false,          // no boolean operation
                    0,              // copy number
                    checkOverlaps); // overlaps checking

  //
  // Reflector
  //
  G4double reflector_innerdiam = 138.5 * mm;
  G4double reflector_outerdiam = 148.5 * mm;
  G4double reflector_length = 139 * mm;
  G4Material *reflector_mat = nist->FindOrBuildMaterial("G4_TEFLON");
  G4ThreeVector reflector_pos = G4ThreeVector();

  auto reflector_solid = new G4Tubs("Reflector",              // name
                                    reflector_innerdiam / 2., // inner radius
                                    reflector_outerdiam / 2., // outer radius
                                    reflector_length / 2.,    // length
                                    0.,                       // start phi angle
                                    360. * deg);              // final phi angle

  auto reflector_logic = new G4LogicalVolume(reflector_solid, reflector_mat, "Reflector");

  new G4PVPlacement(nullptr,         // no rotation
                    reflector_pos,   // at position
                    reflector_logic, // its logical volume
                    "Reflector",     // its name
                    gas_logic,       // its mother  volume
                    false,           // no boolean operation
                    0,               // copy number
                    checkOverlaps);  // overlaps checking

  //
  // Photon detector
  //
  G4double phdet_hx = 148.5 * mm;
  G4double phdet_hy = 148.5 * mm;
  G4double phdet_hz = 1.36 * mm;
  G4Material *phdet_mat = nist->FindOrBuildMaterial("G4_Si");
  G4ThreeVector phdet_pos = G4ThreeVector(0 * mm, 0 * mm, 70.5 * mm);

  auto phdet_solid = new G4Box("PhDet",                                      // its name
                               phdet_hx / 2., phdet_hy / 2., phdet_hz / 2.); // its size

  auto phdet_logic = new G4LogicalVolume(phdet_solid, // its solid
                                         phdet_mat,   // its material
                                         "PhDet");    // its name

  new G4PVPlacement(nullptr,        // no rotation
                    phdet_pos,      // at position
                    phdet_logic,    // its logical volume
                    "PhDet",        // its name
                    gas_logic,      // its mother  volume
                    false,          // no boolean operation
                    0,              // copy number
                    checkOverlaps); // overlaps checking

  // ------------- Optical surfaces --------------
  //
  // Reflector optical surface
  //
  G4OpticalSurface *reflector_os = new G4OpticalSurface("reflectorOpticalSurface");
  reflector_os->SetModel(unified);
  reflector_os->SetType(dielectric_metal);
  reflector_os->SetFinish(ground);
  reflector_os->SetSigmaAlpha(0.01);
  new G4LogicalSkinSurface("Reflector", reflector_logic, reflector_os);

  //
  // Photon detector optical surface
  //
  G4OpticalSurface *phdet_os = new G4OpticalSurface("phdetOpticalSurface");
  phdet_os->SetModel(unified);
  phdet_os->SetType(dielectric_metal);
  phdet_os->SetFinish(polished);
  new G4LogicalSkinSurface("PhDet", phdet_logic, phdet_os);

  // ------------- Material properties --------------
  //
  // Photon energy
  //
  std::vector<G4double> photonenergy = {2.0 * eV, 2.8 * eV, 3.5 * eV, 4. * eV, 6. * eV, 7.2 * eV, 10.0 * eV};

  //
  // Reflectivity
  //
  std::vector<G4double> reflectivity = {.98, .98, .98, .98, .72, .72, .72};
  std::vector<G4double> reflectivity_2 = {0., 0., 0., 0., 0., 0., 0.};

  //
  // Specular lobe
  //
  std::vector<G4double> specularlobe = {0., 0., 0., 0., 0., 0., 0.};

  //
  // Specular spike
  //
  std::vector<G4double> specularspike = {0., 0., 0., 0., 0., 0., 0.};

  //
  // Backscatter
  //
  std::vector<G4double> backscatter = {0., 0., 0., 0., 0., 0., 0.};

  //
  // Refractive index
  //
  std::vector<G4double> refractiveindex = {1, 1, 1, 1, 1, 1, 1};
  std::vector<G4double> refractiveindex_2 = {1.41, 1.41, 1.41, 1.41, 1.41, 1.41, 1.41};

  //
  // Efficiency
  //
  std::vector<G4double> efficiency = {1, 1, 1, 1, 1, 1, 1};

  // ------------- Material property tables --------------
  //
  // Gas MPT1
  //
  auto myMPT1 = new G4MaterialPropertiesTable();
  myMPT1->AddProperty("RINDEX", photonenergy, refractiveindex);
  gas_mat->SetMaterialPropertiesTable(myMPT1);

  //
  // Reflector MPT2
  //
  auto myMPT2 = new G4MaterialPropertiesTable();
  myMPT2->AddProperty("SPECULARLOBECONSTANT", photonenergy, specularlobe);
  myMPT2->AddProperty("SPECULARSPIKECONSTANT", photonenergy, specularspike);
  myMPT2->AddProperty("BACKSCATTERCONSTANT", photonenergy, backscatter);
  myMPT2->AddProperty("RINDEX", photonenergy, refractiveindex_2);
  myMPT2->AddProperty("REFLECTIVITY", photonenergy, reflectivity);
  reflector_mat->SetMaterialPropertiesTable(myMPT2);

  //
  // Photon detector MPT3
  //
  auto myMPT3 = new G4MaterialPropertiesTable();
  myMPT3->AddProperty("REFLECTIVITY", photonenergy, reflectivity_2);
  myMPT3->AddProperty("EFFICIENCY", photonenergy, efficiency);
  phdet_mat->SetMaterialPropertiesTable(myMPT3);

  return physWorld;
}
