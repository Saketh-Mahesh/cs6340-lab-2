/*
 * Copyright Â© 2021 Georgia Institute of Technology (Georgia Tech). All Rights Reserved.
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

#include "Utils.h"

namespace dataflow{
	struct Printer: public FunctionPass {
		static char ID;
		Printer() : FunctionPass(ID){}

	protected:

		/**
	 	 * Print information about each instruction in each function.
		 */
		bool runOnFunction(Function &F) override {
			errs() << "Print information about instructions in " << F.getName() << "\n";
			for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I){
				errs() << "Instruction: " << *I << "\n";
				errs() << "Def set: [";
				if (isDef(&(*I))){
					errs() << *I;
				}
				errs() << "]\n";
				errs() << "Use set: [";
				// Get values used in the instruction.
				for (Use& U : I->operands()){
					Value* v = U.get();
					// Check if Value v is a local variable.
					if(isa<Instruction>(v))
						errs() << *v << ", ";
				}
				errs() << "]\n";
				errs() << "Predecessors: [";
				for(Instruction* P : getPredecessors(&(*I)))
					errs() << *P << ", ";
				errs() << "]\n";
				errs() << "Successors: [";
				for(Instruction* S : getSuccessors(&(*I)))
					errs() << *S << ", ";
				errs() << "]\n\n";
			}
			return false;
		}
	};

	char Printer::ID = 2;
	static RegisterPass<Printer> X("Printer", "Print information about instructions in each function.",
											false /* Only looks at CFG */,
											false /* Analysis Pass */);
}
