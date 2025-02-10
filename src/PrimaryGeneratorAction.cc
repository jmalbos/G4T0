
#include "PrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"


  PrimaryGeneratorAction::PrimaryGeneratorAction()
  {
    G4int n_particle = 1;
    fParticleGun = new G4ParticleGun(n_particle);

    // default particle kinematic

    G4ParticleDefinition *particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle("opticalphoton");

    G4double cosTheta = 2 * G4UniformRand() - 1., phi = 2 * CLHEP::pi * G4UniformRand();
    G4double sinTheta = std::sqrt(1. - cosTheta * cosTheta);
    G4double x0 = sinTheta * std::cos(phi), y0 = sinTheta * std::sin(phi), z0 = cosTheta;

    // fParticleGun->SetParticleMomentumDirection(G4ThreeVector(x0, y0, z0));
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));

    fParticleGun->SetParticleDefinition(particleDefinition);
    fParticleGun->SetParticleEnergy(2.0 * eV);
    fParticleGun->SetNumberOfParticles(1);
    fParticleGun->SetParticlePosition(G4ThreeVector());
  }



  PrimaryGeneratorAction::~PrimaryGeneratorAction()
  {
    delete fParticleGun;
  }


  void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent)
  {
    // this function is called at the begining of ecah event
    //

    // In order to avoid dependence of PrimaryGeneratorAction
    // on DetectorConstruction class we get Envelope volume
    // from G4LogicalVolumeStore.

    G4double worldZHalfLength = 0;
    G4LogicalVolume *worldLV = G4LogicalVolumeStore::GetInstance()->GetVolume("World");
    G4Box *worldBox = nullptr;
    if (worldLV)
      worldBox = dynamic_cast<G4Box *>(worldLV->GetSolid());
    if (worldBox)
      worldZHalfLength = worldBox->GetZHalfLength();
    else
    {
      G4cerr << "World volume of box not found." << G4endl;
      G4cerr << "Perhaps you have changed geometry." << G4endl;
      G4cerr << "The gun will be place in the center." << G4endl;
    }

    // Starting a primary particle close to the world boundary.
    //

    G4cout << "fParticleGun->GeneratePrimaryVertex(anEvent);" << G4endl;

    fParticleGun->GeneratePrimaryVertex(anEvent);
  }


