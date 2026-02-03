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

#pragma once
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/ValueMap.h"
#include "llvm/ADT/SetVector.h"
#include <string>
#include "llvm/IR/InstIterator.h"

using namespace llvm;

namespace dataflow{
	/**
	 * Get the predecessors of a given instruction in the control-flow graph.
	 */
	inline std::vector<Instruction*> getPredecessors(Instruction* I){
		std::vector<Instruction*> ret;
		BasicBlock* BB = I->getParent();
		for(BasicBlock::reverse_iterator i = BB->rbegin(), e = BB->rend(); i != e; ++i){
			if (&(*i) == I){
				++i;
				if(i == e){
					for(pred_iterator pre = pred_begin(BB), BE = pred_end(BB); pre != BE;  ++pre)
						ret.push_back(&(*((*pre)->rbegin())));
				}
				else{
					ret.push_back(&(*i));
				}
				break;
			}
		}
		return ret;
	}

	/**
	 * Get the successors of a given instruction in the control-flow graph.
	 */
	inline std::vector<Instruction*> getSuccessors(Instruction* I){
		std::vector<Instruction*> ret;
		BasicBlock* BB = I->getParent();
		for(BasicBlock::iterator i = BB->begin(), e = BB->end(); i != e; ++i){
			if (&(*i) == I){
				++i;
				if(i == e){
					for(succ_iterator succ = succ_begin(BB), BS = succ_end(BB); succ != BS;  ++succ)
						ret.push_back(&(*((*succ)->begin())));
				}
				else{
					ret.push_back(&(*i));
				}
				break;
			}
		}
		return ret;
	}

	/**
	 * Check whether a given instruction defines a local variable. Note since
	 * LLVM's IR is in SSA, local variables are directly represented by the
	 * instructions defining them.
	 */
	inline bool isDef(Instruction* I){
		return !(I->getType()->isVoidTy());
	}
}
