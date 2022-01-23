#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/ADT/StringRef.h"
#include <string> 
#include <bits/stdc++.h>
#include <iostream>


using namespace std;
using namespace llvm;

#define DEBUG_TYPE "loop"

StringRef *Entry = new StringRef("entry");

namespace {
    
    list<string> t_ref;
    list<string> t_gen;
    list<pair<string, string>> t_equiv;
    list<pair<string, string>> t_equiv_tmp;
    list<pair<string, string>> t_equiv_add;
    list<string> delete_equiv;
    list<pair<string, string>> t_def;
    list<tuple<string, string, string>> flow_dependence;
    list<tuple<string, string, string>> output_dependence;

    int statement = 1;
    set<string> variable = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "x", "y", "p", "pp"};


    struct Dependence : public ModulePass {
        static char ID; // Pass identification, replacement for typeid
        Dependence() : ModulePass(ID) {}

        void delete_useless(){
            // errs() << "EQUIV_ADD:{";
            // for (auto iter = t_equiv_add.begin(); iter != t_equiv_add.end(); iter++) {
            //     if (iter != t_equiv_add.begin()) errs() << ", ";
            //     errs() << '(' << iter->first << ", " << iter->second << ')';
            // }  
            // errs() << "}\n";


            for (auto iter_equiv_outter = t_equiv_add.begin(); iter_equiv_outter != t_equiv_add.end(); iter_equiv_outter++){
                delete_equiv.push_back(iter_equiv_outter->first);
                delete_equiv.push_back(iter_equiv_outter->second);
            }

            while(!delete_equiv.empty()){
                for(auto iter = delete_equiv.begin(); iter != delete_equiv.end(); iter++){
                    bool flag = false;
                    for(auto iter_equiv = t_equiv.begin(); iter_equiv != t_equiv.end(); iter_equiv++){
                        if(!(*iter).compare(iter_equiv->first)){
                            delete_equiv.push_back(iter_equiv->second);
                            t_equiv.remove(*iter_equiv);
                            flag = true;
                            break;
                        }
                        else if(!(*iter).compare(iter_equiv->second)){
                            delete_equiv.push_back(iter_equiv->first);
                            t_equiv.remove(*iter_equiv);
                            flag = true;
                            break;
                        }
                    }
                    if (!flag) {
                        delete_equiv.remove(*iter);
                        break;
                    }
                }
            }
            t_equiv.merge(t_equiv_add);
            t_equiv_add.clear();
        }

        void add_equiv(){
            // errs() << "MergeADD:{";
            // for (auto iter = t_equiv_add.begin(); iter != t_equiv_add.end(); iter++) {
            //     if (iter != t_equiv_add.begin()) errs() << ", ";
            //     errs() << '(' << iter->first << ", " << iter->second << ')';
            // }  
            // errs() << "}\n";
            // errs() << "BeforeADDEquiv:{";
            // for (auto iter = t_equiv.begin(); iter != t_equiv.end(); iter++) {
            //     if (iter != t_equiv.begin()) errs() << ", ";
            //     errs() << '(' << iter->first << ", " << iter->second << ')';
            // }  
            // errs() << "}\n";
            // update t_equiv find subtree (add)
            for (auto iter_equiv_outter = t_equiv.begin(); iter_equiv_outter != t_equiv.end(); iter_equiv_outter++){

                for (auto iter_equiv = t_equiv.begin(); iter_equiv != t_equiv.end(); iter_equiv++) {
                    if(!iter_equiv_outter->first.substr(1, 10).compare(iter_equiv->first)){
                        
                        auto current_pair = make_pair("*" + iter_equiv->second, iter_equiv_outter->second);
                        if(find(t_equiv.begin(), t_equiv.end(), current_pair) == t_equiv.end()){
                            t_equiv_tmp.push_back(current_pair);
                        }
                    }
                    else if (!iter_equiv_outter->first.substr(1, 10).compare(iter_equiv->second)){
                        
                        auto current_pair = make_pair("*" + iter_equiv->first, iter_equiv_outter->second);
                        if(find(t_equiv.begin(), t_equiv.end(), current_pair) == t_equiv.end()){
                            t_equiv_tmp.push_back(current_pair);
                        }
                    }
                    else if (!iter_equiv_outter->second.substr(1, 10).compare(iter_equiv->first)){
                        
                        auto current_pair = make_pair("*" + iter_equiv->second, iter_equiv_outter->first);
                        if(find(t_equiv.begin(), t_equiv.end(), current_pair) == t_equiv.end()){
                            t_equiv_tmp.push_back(current_pair);
                        }
                    }
                    else if (!iter_equiv_outter->second.substr(1, 10).compare(iter_equiv->second)){
                        
                        auto current_pair = make_pair("*" + iter_equiv->first, iter_equiv_outter->first);
                        if(find(t_equiv.begin(), t_equiv.end(), current_pair) == t_equiv.end()){
                            t_equiv_tmp.push_back(current_pair);
                        }
                    }
                }
            } 

            // errs() << "TEQUIV_TMP:{";
            // for (auto iter = t_equiv_tmp.begin(); iter != t_equiv_tmp.end(); iter++) {
            //     if (iter != t_equiv_tmp.begin()) errs() << ", ";
            //     errs() << '(' << iter->first << ", " << iter->second << ')';
            // }  
            // errs() << "}\n";
            // errs() << "TEQUIV:{";
            // for (auto iter = t_equiv.begin(); iter != t_equiv.end(); iter++) {
            //     if (iter != t_equiv.begin()) errs() << ", ";
            //     errs() << '(' << iter->first << ", " << iter->second << ')';
            // }  
            // errs() << "}\n";
            

            t_equiv.merge(t_equiv_tmp);
            
            t_equiv_tmp.clear();

            // errs() << "----------------\n";
        }


        void update_list_for_equiv(){
            //t_def
            for (list<string>::iterator iter = t_gen.begin(); iter != t_gen.end(); iter++) {
                auto current_pair = make_pair(*iter, "S" + to_string(statement));
                t_def.push_back(current_pair);
            }

            // check t_equiv for ref
            for (auto iter_ref = t_ref.begin(); iter_ref != t_ref.end(); iter_ref++){
                    // errs() << *iter_ref << '\n';
                for (auto iter_equiv = t_equiv.begin(); iter_equiv != t_equiv.end(); iter_equiv++) {
                    // errs() << iter_equiv->second << '\n';
                    if (!(iter_equiv->first.compare(*iter_ref))) 
                        t_ref.push_front(iter_equiv->second);
                    // else if (!(iter_equiv->second.compare(*iter_ref)))
                    //     t_ref.push_front(iter_equiv->first);
                }
            } 
            // check t_equiv for gen, also add into def
            for (auto iter_gen = t_gen.begin(); iter_gen != t_gen.end(); iter_gen++){
                    // errs() << *iter_gen << '\n';
                for (auto iter_equiv = t_equiv.begin(); iter_equiv != t_equiv.end(); iter_equiv++) {
                    // errs() << iter_equiv->second << '\n';
                    if (!(iter_equiv->first.compare(*iter_gen))) {
                        t_gen.push_front(iter_equiv->second);
                        auto current_pair = make_pair(iter_equiv->second, "S" + to_string(statement));
                        t_def.push_front(current_pair);
                    }
                        
                    // else if (!(iter_equiv->second.compare(*iter_gen))) {
                    //     t_gen.push_front(iter_equiv->first);
                    //     auto current_pair = make_pair(iter_equiv->first, "S" + to_string(statement));
                    //     t_def.push_front(current_pair);
                    // }
                        
                }
            } 

            
            
            // bool break_loop = false;
            // check t_equiv for def
            // for (auto iter_def = t_def.begin(); iter_def != t_def.end(); iter_def++){
            //         // errs() << *iter_gen << '\n';
            //     for (auto iter_equiv = t_equiv.begin(); iter_equiv != t_equiv.end(); iter_equiv++) {
            //         // errs() << iter_equiv->second << '\n';
            //         if (!(iter_equiv->first.compare(iter_def->first))) {

            //             for (auto iter = t_def.begin(); iter != t_def.end(); iter++){
            //                 if (!(iter_equiv->second).compare(iter->first)) {
            //                     break_loop = true;
            //                     break;
            //                 }
            //             }
            //             if (break_loop) break;
            //             auto current_pair = make_pair(iter_equiv->second, "S" + to_string(statement));
            //             t_def.push_front(current_pair);
            //         }
            //         else if (!(iter_equiv->second.compare(iter_def->first))) {

            //             for (auto iter = t_def.begin(); iter != t_def.end(); iter++){
            //                 if (!(iter_equiv->first).compare(iter->first)) {
            //                     break_loop = true;
            //                     break;
            //                 }
            //             }
            //             if (break_loop) break;
            //             auto current_pair =make_pair(iter_equiv->first, "S" + to_string(statement));
            //             t_def.push_front(current_pair);
            //         }
            //     }
            // } 
        }


        void print_data_flow(){
            // print result
            errs() << "S" + to_string(statement) + '\n';

            errs() << "TREF:{";
            for (list<string>::iterator iter = t_ref.begin(); iter != t_ref.end(); iter++) {
                if (iter != t_ref.begin()) errs() << ", ";
                errs() << *iter;
            }  
            errs() << "}\n";

            errs() << "TGEN:{";
            for (list<string>::iterator iter = t_gen.begin(); iter != t_gen.end(); iter++) {
                if (iter != t_gen.begin()) errs() << ", ";
                errs() << *iter;
            }  
            errs() << "}\n";

            errs() << "TDEF:{";
            for (auto iter = t_def.begin(); iter != t_def.end(); iter++) {
                if (iter != t_def.begin()) errs() << ", ";
                errs() << '(' << iter->first << ", " << iter->second << ')';
            }  
            errs() << "}\n";

            errs() << "TEQUIV:{";
            for (auto iter = t_equiv.begin(); iter != t_equiv.end(); iter++) {
                if (iter != t_equiv.begin()) errs() << ", ";
                errs() << '(' << iter->first << ", " << iter->second << ')';
            }  
            errs() << "}\n";


        }

        void print_dependence_analyzer(){
            //flow dependence
            for (auto iter_ref = t_ref.begin(); iter_ref != t_ref.end(); iter_ref++){
                    // errs() << *iter_ref << '\n';
                for (auto iter_def = t_def.begin(); iter_def != t_def.end(); iter_def++) {
                    // errs() << iter_def->first << '\n';
                    if (!(iter_def->first.compare(*iter_ref))){
                        auto current_pair = make_tuple(*iter_ref, iter_def->second, "S" + to_string(statement));
                        flow_dependence.push_back(current_pair);
                    } 
                }
            }

            //output dependence
            for (auto iter_gen = t_gen.begin(); iter_gen != t_gen.end(); iter_gen++){
                    // errs() << *iter_ref << '\n';
                for (auto iter_def = t_def.begin(); iter_def != t_def.end(); iter_def++) {
                    // errs() << iter_def->second << '\n';
                    if (!(iter_def->first.compare(*iter_gen)) && iter_def->second.compare("S" + to_string(statement))){
                        auto current_pair = make_tuple(*iter_gen, iter_def->second, "S" + to_string(statement));
                        output_dependence.push_back(current_pair);
                    } 
                }
            }

            //print result
            errs() << "DEP: {\n";
            for (const auto& i : flow_dependence) {
                errs() << get<0>(i) << ": " << get<1>(i) << "------->" << get<2>(i) << '\n';
            }
            for (const auto& i : output_dependence) {
                errs() << get<0>(i) << ": " << get<1>(i) << "---O--->" << get<2>(i) << '\n';
            }
            errs() << "}\n";

            errs() << "\n";

        }

        void data_flow_analyzer(){
            delete_useless();
            add_equiv();
            
            update_list_for_equiv();
            print_data_flow();
            print_dependence_analyzer();

            t_ref.clear();
            t_gen.clear();
            flow_dependence.clear();
            output_dependence.clear();
        }

    
        void load_store_match(Function::iterator BB){ 

            if(!BB->getName().find(*Entry, 0)){ // inside Entry
                
                bool load_pointer_flag = false; // for case *p = a, would load and store
                string load_pointer_type = "";
                string load_pointer_name = "";

                for (BasicBlock::iterator itrIns = (*BB).begin(); itrIns != (*BB).end(); itrIns++){
                    if(dyn_cast<Instruction>(itrIns)){
                        if(const LoadInst *LI = dyn_cast<LoadInst>(itrIns)){  // match load

                            Value *val0 = itrIns->getOperand(0);
                            // Value *val1 = itrIns->getOperand(1);

                            // errs() << val0->getName() << " " << *val0->getType() << "\n";
                            

                            auto name = val0->getName();

                            if(name.compare("")){
                                t_ref.push_back(name);
                                // errs() << "Load: " << name << '\n';
                                
                                //get type
                                std::string type_str;
                                llvm::raw_string_ostream rso(type_str);
                                (val0->getType())->print(rso);
                                // errs()<<rso.str();
                                string type = rso.str();
                                if(type.compare("i32*")){ // for case *p = a, would load and store
                                    load_pointer_flag = true;
                                    load_pointer_type = type;
                                    load_pointer_name = val0->getName().str();

                                    // errs() << "load_pointer_name: " << load_pointer_name << ", load_pointer_type: " << load_pointer_type << "\n";
                                }
                            }
                        }
                        if(const StoreInst *SI =dyn_cast<StoreInst>(itrIns)){ 
                            
                            Value *val0 = itrIns->getOperand(0);
                            Value *val1 = itrIns->getOperand(1);

                            // if (variable.find(val0->getName().str()) != variable.end()) errs() << "store val0 " << val0->getName().str() << '\n'; 

                            if(variable.find(val0->getName().str()) != variable.end() && variable.find(val1->getName().str()) != variable.end()) {  // for case p = &a, would store with variable
                                //get type
                                std::string type_str;
                                llvm::raw_string_ostream rso(type_str);
                                (val1->getType())->print(rso);
                                // errs()<<rso.str().substr(4, 8);
                                string type = rso.str();
                                
                                std::string type_str_2;
                                llvm::raw_string_ostream rso2(type_str_2);
                                (val0->getType())->print(rso2);
                                string type_2 = rso2.str();
                                
                                // errs() << *val1->getType() << '\n';
                                // errs() << *val0->getType() << '\n';
                                // string val1_pointer = type+ val1->getName().str();  // ex *p (rso.str() is type)  // left-hand side
                                // string val0_pointer = type_2 + val0->getName().str(); // right-hand side
                                // errs() << val1_pointer << ' ' << val0_pointer << '\n';
                                int num = type.length() - type_2.length();
                                // errs() << num << '\n';
                                std::string stuff(num, '*');
                                // errs() << stuff << '\n';

                                string val1_pointer = stuff + val1->getName().str();

                                auto current_pair = make_pair(val1_pointer, val0->getName().str());   // t_equiv
                                t_equiv_add.push_back(current_pair);

                            }


                            if (load_pointer_flag && variable.find(val0->getName().str()) != variable.end())  {   // *p = &a
                                auto pointer_name = load_pointer_type.substr(4, 8) + load_pointer_name;
                                auto current_pair = make_pair(pointer_name, val0->getName().str());
                                t_equiv_add.push_back(current_pair);

                                // remove useless pointer

                            }
                            
                            if(load_pointer_flag){  // for case *p = a, would load and store
                                // errs() << load_pointer_name << " " << load_pointer_type << "\n";
                                // auto name = load_pointer_type.substr(4, 8) + load_pointer_name;
                                int num = load_pointer_type.substr(4, 8).length();
                                // errs() << num << '\n';
                                std::string stuff(num, '*');
                                auto name = stuff + load_pointer_name;
                                t_gen.push_back(name);
                            }
                            else {
                                // errs() << val1->getName() << " " << *val1->getType() << "\n";
                                auto name = val1->getName();
                                t_gen.push_back(name);
                            }
                            
                            load_pointer_flag = false;

                            data_flow_analyzer();
                                                      
                            statement++;
                        }
                    } 
                }
            }
        }


        virtual bool runOnModule(Module &M) { //override ModulePass            
            for (Module::iterator F = M.begin(); F != M.end(); F++){ // in Module
                for (Function::iterator BB = (*F).begin(); BB != (*F).end(); BB++) { // In Function
                    load_store_match(BB);
                }
            }

            return false;
        }
    };
 }

//initialize identifier
char Dependence::ID = 0;
//"dependence" is the name of pass
//"Simple program to find data dependency " is the explaination of your pass
static RegisterPass<Dependence> GS("dependence", "Simple program to find data dependency");