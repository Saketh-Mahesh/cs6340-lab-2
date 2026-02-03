/*
 * Copyright Â© 2023 Georgia Institute of Technology (Georgia Tech). All Rights Reserved.
 * Template code for CS 6340 Software Analysis
 * Instructors: Mayur Naik and Chris Poch
 * Head TAs: Kelly Parks and Joel Cooper
 *
 * Georgia Tech asserts copyright ownership of this template and all derivative
 * works, including solutions to the projects assigned in this course. Students
 * and other users of this template code are advised not to share it with others
 * or to make it available on publicly viewable websites including repositories
 * such as GitHub and GitLab. This copyright statement should not be removed
 * or edited. Removing it will be considered an academic integrity issue.
 *
 * We do grant permission to share solutions privately with non-students such
 * as potential employers as long as this header remains in full. However, 
 * sharing with other current or future students or using a medium to share
 * where the code is widely available on the internet is prohibited and 
 * subject to being investigated as a GT honor code violation.
 * Please respect the intellectual ownership of the course materials 
 * (including exam keys, project requirements, etc.) and do not distribute them 
 * to anyone not enrolled in the class. Use of any previous semester course 
 * materials, such as tests, quizzes, homework, projects, videos, and any other 
 * coursework, is prohibited in this course. */

#include "AnalysisStrategy.h"
#include "IChaoticIterationAlgorithm.h"
#include "ChaoticIterationAlgorithm.cpp"

using namespace llvm;

namespace dataflow {
	bool AnalysisStrategy::runOnFunction(Function &F){
		errs() << "Running " << getAnalysisName() << " on " << F.getName() << "\n";

		ValueMap<Instruction*,SetVector<Value*>*> gradedInMap;
		ValueMap<Instruction*,SetVector<Value*>*> gradedOutMap;

		for (inst_iterator I = inst_begin(F), E= inst_end(F); I != E; ++I){
			gradedInMap[&(*I)] = new SetVector<Value*>;
			gradedOutMap[&(*I)] = new SetVector<Value*>;

			inMap[&(*I)] = gradedInMap[&(*I)];
			outMap[&(*I)] = gradedOutMap[&(*I)];
		}

		ChaoticIterationAlgorithm chaoticIterationAlgorithm;
		chaoticIterationAlgorithm.run(this, F);
		
		for (inst_iterator I = inst_begin(F), E= inst_end(F); I != E; ++I){
			errs() << "Instruction: " << *I << "\n";
			errs() << "In set: \n";
			SetVector<Value*>* inSet = gradedInMap[&(*I)];
			errs() << "[";
			for(SetVector<Value*>::iterator V = inSet->begin(), VE = inSet->end(); V != VE; ++V){
				errs() << **V << "; ";
			}
			errs() << "]\n";
			errs() << "Out set: \n";
			SetVector<Value*>* outSet = gradedOutMap[&(*I)];
			errs() << "[";
			for(SetVector<Value*>::iterator V = outSet->begin(), VE = outSet->end(); V != VE; ++V){
				errs() << **V << "; ";
			}
			errs() << "]\n";
			errs() << "\n";
		}
		
		for (inst_iterator I = inst_begin(F), E= inst_end(F); I != E; ++I){
			delete gradedInMap[&(*I)];
			delete gradedOutMap[&(*I)];
		}
		return false;
	}
}
