
/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "search_keynodes.h"
#include "search.h"

#include "../sc-common/sc_keynodes.h"

#include "sc-core/sc_helper.h"
#include "sc-core/sc_memory_headers.h"

#include <glib.h>

sc_addr keynode_question_search_atomic_commands;
sc_addr keynode_question_search_all_nodes_in_set;
sc_addr keynode_question_search_tooltip;

sc_addr keynode_question_all_output_const_pos_arc;
sc_addr keynode_question_all_input_const_pos_arc;
sc_addr keynode_question_all_output_const_pos_arc_with_rel;
sc_addr keynode_question_all_input_const_pos_arc_with_rel;
sc_addr keynode_question_full_semantic_neighborhood;
sc_addr keynode_question_decomposition;
sc_addr keynode_question_all_identifiers;
sc_addr keynode_question_all_identified_elements;
sc_addr keynode_question_search_all_superclasses_in_quasybinary_relation;
sc_addr keynode_question_search_all_subclasses_in_quasybinary_relation;
sc_addr keynode_question_search_links_of_relation_connected_with_element;
sc_addr keynode_question_search_full_pattern;

sc_addr keynode_ui_main_menu;
sc_addr keynode_ui_user_command_class_atom;
sc_addr keynode_nrel_ui_commands_decomposition;
sc_addr keynode_sc_definition;
sc_addr keynode_sc_explanation;
sc_addr keynode_sc_note;
sc_addr keynode_nrel_sc_text_translation_value;

sc_addr keynode_question;
sc_addr keynode_question_initiated;
sc_addr keynode_nrel_answer;
sc_addr keynode_question_finished;
sc_addr keynode_question_finished_successfully;
sc_addr keynode_question_finished_unsuccessfully;
sc_addr keynode_quasybinary_relation;
sc_addr keynode_decomposition_relation;
sc_addr keynode_taxonomy_relation;
sc_addr keynode_identification_relation;
sc_addr keynode_order_relation;
sc_addr keynode_nonbinary_relation;
sc_addr keynode_typical_sc_neighborhood;
sc_addr keynode_sc_neighborhood;

sc_addr keynode_nrel_identification;
sc_addr keynode_nrel_translation;
sc_addr keynode_nrel_main_idtf;
sc_addr keynode_rrel_key_sc_element;
sc_addr keynode_nrel_key_sc_element_base_order;
sc_addr keynode_nrel_inclusion;
sc_addr keynode_nrel_strict_inclusion;

sc_addr keynode_system_element;
sc_addr keynode_languages;

sc_addr keynode_rrel_1;
sc_addr keynode_rrel_2;

const char keynode_question_search_atomic_commands_str[] = "question_search_atomic_commands";
const char keynode_question_search_all_nodes_in_set_str[] = "question_search_all_nodes_in_set";
const char keynode_question_search_tooltip_str[] = "question_search_tooltip";

char const keynode_question_all_output_const_pos_arc_str[] = "question_search_all_output_const_pos_arcs";
char const keynode_question_all_input_const_pos_arc_str[] = "question_search_all_input_const_pos_arcs";
char const keynode_question_all_output_const_pos_arc_with_rel_str[] =
    "question_search_all_output_const_pos_arcs_with_rel";
char const keynode_question_all_input_const_pos_arc_with_rel_str[] =
    "question_search_all_input_const_pos_arcs_with_rel";
char const keynode_question_full_semantic_neighborhood_str[] = "question_search_full_semantic_neighborhood";
char const keynode_question_decomposition_str[] = "question_search_decomposition";
char const keynode_question_all_identifiers_str[] = "question_search_all_identifiers";
char const keynode_question_all_identified_elements_str[] = "question_search_all_identified_elements";
char const keynode_question_search_full_pattern_str[] = "question_full_pattern_search";

char const keynode_question_search_all_subclasses_in_quasybinary_relation_str[] =
    "question_search_all_subclasses_in_quasybinary_relation";
char const keynode_question_search_all_superclasses_in_quasybinary_relation_str[] =
    "question_search_all_superclasses_in_quasybinary_relation";
char const keynode_question_search_links_of_relation_connected_with_element_str[] =
    "question_search_links_of_relation_connected_with_element";

char const keynode_question_initiated_str[] = "question_initiated";
char const keynode_question_str[] = "question";
char const keynode_rrel_1_str[] = "rrel_1";
char const keynode_rrel_2_str[] = "rrel_2";
char const keynode_nrel_answer_str[] = "nrel_answer";
char const keynode_question_finished_str[] = "question_finished";
char const keynode_question_finished_successfully_str[] = "question_finished_successfully";
char const keynode_question_finished_unsuccessfully_str[] = "question_finished_unsuccessfully";
char const keynode_quasybinary_relation_str[] = "quasybinary_relation";
char const keynode_decomposition_relation_str[] = "decomposition_relation";
char const keynode_taxonomy_relation_str[] = "taxonomy_relation";
char const keynode_identification_relation_str[] = "identification_relation";
char const keynode_order_relation_str[] = "order_relation";
char const keynode_nonbinary_relation_str[] = "nonbinary_relation";
char const keynode_typical_sc_neighborhood_str[] = "typical_semantic_neighbourhood";

const char keynode_ui_main_menu_str[] = "keynode_ui_main_menu";
const char keynode_ui_user_command_class_atom_str[] = "keynode_ui_user_command_class_atom";
const char keynode_nrel_ui_commands_decomposition_str[] = "keynode_nrel_ui_commands_decomposition";
const char keynode_sc_definition_str[] = "keynode_sc_definition";
const char keynode_sc_explanation_str[] = "keynode_sc_explanation";
const char keynode_sc_note_str[] = "keynode_sc_note";
const char keynode_nrel_sc_text_translation_value_str[] = "keynode_nrel_sc_text_translation_value";

char const keynode_nrel_identification_str[] = "nrel_identification";
char const keynode_nrel_translation_str[] = "nrel_sc_text_translation";
char const keynode_nrel_main_idtf_str[] = "nrel_main_idtf";
char const keynode_nrel_inclusion_str[] = "nrel_inclusion";
char const keynode_nrel_strict_inclusion_str[] = "nrel_strict_inclusion";
char const keynode_rrel_key_sc_element_str[] = "rrel_key_sc_element";
char const keynode_nrel_key_sc_element_base_order_str[] = "nrel_key_sc_element_base_order";
char const keynode_system_element_str[] = "system_element";
char const keynode_sc_neighborhood_str[] = "semantic_neighbourhood";
char const keynode_languages_str[] = "languages";

sc_result search_keynodes_initialize(sc_memory_context * ctx, sc_addr const init_memory_generated_structure)
{
  RESOLVE_KEYNODE(ctx, keynode_question_all_output_const_pos_arc, init_memory_generated_structure);
  RESOLVE_KEYNODE(ctx, keynode_question_all_input_const_pos_arc, init_memory_generated_structure);
  RESOLVE_KEYNODE(ctx, keynode_question_all_output_const_pos_arc_with_rel, init_memory_generated_structure);
  RESOLVE_KEYNODE(ctx, keynode_question_all_input_const_pos_arc_with_rel, init_memory_generated_structure);
  RESOLVE_KEYNODE(ctx, keynode_question_full_semantic_neighborhood, init_memory_generated_structure);
  RESOLVE_KEYNODE(ctx, keynode_question_decomposition, init_memory_generated_structure);
  RESOLVE_KEYNODE(ctx, keynode_nrel_answer, init_memory_generated_structure);
  RESOLVE_KEYNODE(ctx, keynode_question_all_identifiers, init_memory_generated_structure);
  RESOLVE_KEYNODE(ctx, keynode_question_all_identified_elements, init_memory_generated_structure);
  RESOLVE_KEYNODE(ctx, keynode_question_search_all_subclasses_in_quasybinary_relation, init_memory_generated_structure);
  RESOLVE_KEYNODE(
      ctx, keynode_question_search_all_superclasses_in_quasybinary_relation, init_memory_generated_structure);
  RESOLVE_KEYNODE(
      ctx, keynode_question_search_links_of_relation_connected_with_element, init_memory_generated_structure);
  RESOLVE_KEYNODE(ctx, keynode_question_search_full_pattern, init_memory_generated_structure);

  RESOLVE_KEYNODE(ctx, keynode_question_initiated, init_memory_generated_structure);
  RESOLVE_KEYNODE(ctx, keynode_question, init_memory_generated_structure);
  RESOLVE_KEYNODE(ctx, keynode_question_finished, init_memory_generated_structure);
  RESOLVE_KEYNODE(ctx, keynode_question_finished_successfully, init_memory_generated_structure);
  RESOLVE_KEYNODE(ctx, keynode_question_finished_unsuccessfully, init_memory_generated_structure);
  RESOLVE_KEYNODE(ctx, keynode_quasybinary_relation, init_memory_generated_structure);
  RESOLVE_KEYNODE(ctx, keynode_decomposition_relation, init_memory_generated_structure);
  RESOLVE_KEYNODE(ctx, keynode_taxonomy_relation, init_memory_generated_structure);
  RESOLVE_KEYNODE(ctx, keynode_identification_relation, init_memory_generated_structure);
  RESOLVE_KEYNODE(ctx, keynode_order_relation, init_memory_generated_structure);
  RESOLVE_KEYNODE(ctx, keynode_nonbinary_relation, init_memory_generated_structure);
  RESOLVE_KEYNODE(ctx, keynode_typical_sc_neighborhood, init_memory_generated_structure);
  RESOLVE_KEYNODE(ctx, keynode_sc_neighborhood, init_memory_generated_structure);
  RESOLVE_KEYNODE(ctx, keynode_question_search_atomic_commands, init_memory_generated_structure);
  RESOLVE_KEYNODE(ctx, keynode_question_search_all_nodes_in_set, init_memory_generated_structure);
  RESOLVE_KEYNODE(ctx, keynode_question_search_tooltip, init_memory_generated_structure);

  RESOLVE_KEYNODE(ctx, keynode_ui_main_menu, init_memory_generated_structure);
  RESOLVE_KEYNODE(ctx, keynode_ui_user_command_class_atom, init_memory_generated_structure);
  RESOLVE_KEYNODE(ctx, keynode_nrel_ui_commands_decomposition, init_memory_generated_structure);

  RESOLVE_KEYNODE(ctx, keynode_sc_definition, init_memory_generated_structure);
  RESOLVE_KEYNODE(ctx, keynode_sc_explanation, init_memory_generated_structure);
  RESOLVE_KEYNODE(ctx, keynode_sc_note, init_memory_generated_structure);
  RESOLVE_KEYNODE(ctx, keynode_nrel_sc_text_translation_value, init_memory_generated_structure);

  RESOLVE_KEYNODE(ctx, keynode_rrel_1, init_memory_generated_structure);
  RESOLVE_KEYNODE(ctx, keynode_rrel_2, init_memory_generated_structure);

  RESOLVE_KEYNODE(ctx, keynode_nrel_identification, init_memory_generated_structure);
  RESOLVE_KEYNODE(ctx, keynode_nrel_translation, init_memory_generated_structure);
  RESOLVE_KEYNODE(ctx, keynode_nrel_main_idtf, init_memory_generated_structure);
  RESOLVE_KEYNODE(ctx, keynode_rrel_key_sc_element, init_memory_generated_structure);
  RESOLVE_KEYNODE(ctx, keynode_nrel_key_sc_element_base_order, init_memory_generated_structure);
  RESOLVE_KEYNODE(ctx, keynode_nrel_inclusion, init_memory_generated_structure);
  RESOLVE_KEYNODE(ctx, keynode_nrel_strict_inclusion, init_memory_generated_structure);
  RESOLVE_KEYNODE(ctx, keynode_system_element, init_memory_generated_structure);
  RESOLVE_KEYNODE(ctx, keynode_languages, init_memory_generated_structure);

  return SC_RESULT_OK;
}
