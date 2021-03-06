// gemc headers
#include "material_factory.h"
#include "cpp_materials.h"

// G4 headers
#include "G4Element.hh"
#include "G4NistManager.hh"
#include "G4OpBoundaryProcess.hh"


// CLHEP units
#include "CLHEP/Units/PhysicalConstants.h"
using namespace CLHEP;


map<string, G4Material*> cpp_materials::initMaterials(runConditions rc, goptions opts)
{
	
	// Many of these elements are already in the G4 Material Database as materials:
	// http://geant4.cern.ch/UserDocumentation/UsersGuides/ForApplicationDeveloper/html/apas08.html
	// When building materials using fractional masses, one can use the G4 Materials Database
	//
	// Elements are still needed when materials are formed using
	// molecular components
	// Example for Water:
	// H2O->AddElement(elH, natoms=2);
	// H2O->AddElement(elO, natoms=1);
	
	
	map<string, G4Material*> MMats;
	
	
	G4double a, z, density;
	G4int nel;
	G4String symbol;

	

	G4NistManager* matman = G4NistManager::Instance();
	

	G4Material *Air_Opt = new G4Material("Air_Opt",   density= 1.29*mg/cm3, nel=2);
	Air_Opt->AddMaterial(matman->FindOrBuildMaterial("G4_N"), 70.*perCent);
	Air_Opt->AddMaterial(matman->FindOrBuildMaterial("G4_O"), 30.*perCent);
	
	// this material will kill every tracks that touch it
	G4Material *Kryptonite = new G4Material("Kryptonite", density= 0.00000001*mg/cm3, nel=1);
	Kryptonite->AddMaterial(matman->FindOrBuildMaterial("G4_Ar"), 100.*perCent);

	// aluminum honeycomb core  (it is actually made of alloy Alu-Alloy 3003 (AlMnCu) - still used by HPS ECAL
	G4Material *AlHoneycomb = new G4Material("AlHoneycomb", z=13, a=  26.982*g/mole, density =  0.13*g/cm3);

	// Various Vacuums
	// 1 torr is 1/760 atmospheric pressure, that is 1.29*mg/cm3
	G4Material *vacuum_m9 = new G4Material("vacuum_m9", density= 1.68e-12*mg/cm3, nel=2);
	vacuum_m9->AddMaterial(matman->FindOrBuildMaterial("G4_N"), 70.*perCent);
	vacuum_m9->AddMaterial(matman->FindOrBuildMaterial("G4_O"), 30.*perCent);
	
	G4Material *vacuum_m3 = new G4Material("vacuum_m3", density= 1.68e-6*mg/cm3, nel=2);
	vacuum_m3->AddMaterial(matman->FindOrBuildMaterial("G4_N"), 70.*perCent);
	vacuum_m3->AddMaterial(matman->FindOrBuildMaterial("G4_O"), 30.*perCent);
	

	
	MMats["Air_Opt"]        = Air_Opt;
	MMats["Vacuum"]         = matman->FindOrBuildMaterial("G4_Galactic");
	MMats["vacuum_m9"]      = vacuum_m9;
	MMats["vacuum_m3"]      = vacuum_m3;
	MMats["AlHoneycomb"]    = AlHoneycomb;

	
	// Materials Optical Properties
	
	// Air Reflection
	const G4int nEntries_Air = 2;
	G4double PhotonEnergy_Air[nEntries_Air]    = { 2.034*eV , 4.136*eV };
	G4double RefractiveIndex_Air[nEntries_Air] = { 1.00, 1.00 };
	
	G4MaterialPropertiesTable* Air_MPT = new G4MaterialPropertiesTable();
	Air_MPT->AddProperty("RINDEX", PhotonEnergy_Air, RefractiveIndex_Air, nEntries_Air);
	MMats["Air_Opt"]->SetMaterialPropertiesTable(Air_MPT);

	return MMats;
}
