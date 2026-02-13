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

namespace dataflow {
	struct LivenessAnalysis : public AnalysisStrategy{
		public:
			static char ID;
			LivenessAnalysis() : AnalysisStrategy(ID){}
			virtual ~LivenessAnalysis() {}

			void constructInMap(Instruction* current) {
				for (Use& U : current->operands()){
					Value* v = U.get();
					if(isa<Instruction>(v))
						inMap[current]->insert(v);
				}

				for (Value* v : *outMap[current]) {
					if (v != current) {
						inMap[current]->insert(v);
					}
				}
			}

			void constructOutMap(Instruction* current) {
				std::vector<Instruction*> successors = getSuccessors(current);
				for (Instruction* successor : successors) {
					SetVector<Value*>* succesor_in = inMap[successor];
					if (succesor_in) {
						outMap[current]->insert(succesor_in->begin(), succesor_in->end());
					}
				}
			}

			/**
			 * Implement your analysis in this function. Store your results in AnalysisStrategy::inMap and
			 * AnalysisStrategy:outMap.  The return result may be used by your ChaoticIterationAlgorithm
			 * as needed, but is not required to be used.  If you are going to use the result, return
			 * either Modified or Unmodified as appropriate.  If you are not using the result, return
			 * NotApplicable.
			 */
			virtual EvaluationResult evaluate(Instruction* current) override {
				bool modified = false;
				size_t inMapBefore = inMap[current]->size();
				size_t outMapBefore = outMap[current]->size();

				constructOutMap(current);

				if (outMap[current]->size() > outMapBefore) {
					modified = true;
				}

				constructInMap(current);
				if (inMap[current]->size() > inMapBefore)
					modified = true;

				return modified ? EvaluationResult::Modified : EvaluationResult::Unmodified;

			}

		protected:
			virtual std::string getAnalysisName() override{
				return "Liveness";
			}
	};

	char LivenessAnalysis::ID = 1;
	static RegisterPass<LivenessAnalysis> X("Liveness", "Liveness Analysis Strategy",
											false /* Only looks at CFG */,
											false /* Analysis Pass */);
}
