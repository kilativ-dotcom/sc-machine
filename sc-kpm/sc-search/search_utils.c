/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "search_utils.h"
#include "search_keynodes.h"
#include "search_defines.h"
#include "search.h"

#include "sc-core/sc_helper.h"

sc_addr create_answer_node()
{
  sc_addr res = sc_memory_node_new(s_default_ctx, sc_type_node | sc_type_const);
  SYSTEM_ELEMENT(res);
  return res;
}

void connect_answer_to_action(sc_addr action, sc_addr answer)
{
  sc_addr arc = sc_memory_arc_new(s_default_ctx, sc_type_arc_common | sc_type_const, action, answer);
  SYSTEM_ELEMENT(arc);
  arc = sc_memory_arc_new(s_default_ctx, sc_type_arc_pos_const_perm, keynode_nrel_answer, arc);
  SYSTEM_ELEMENT(arc);
}

void appendIntoAnswer(sc_addr answer, sc_addr el)
{
  sc_addr arc;
  if (sc_helper_check_arc(s_default_ctx, answer, el, sc_type_arc_pos_const_perm) == SC_TRUE)
    return;

  arc = sc_memory_arc_new(s_default_ctx, sc_type_arc_pos_const_perm, answer, el);
  SYSTEM_ELEMENT(arc);
}

void finish_action(sc_addr action)
{
  sc_addr arc;

  arc = sc_memory_arc_new(s_default_ctx, sc_type_arc_pos_const_perm, keynode_action_finished, action);
  SYSTEM_ELEMENT(arc);
}

void finish_action_successfully(sc_memory_context * ctx, sc_addr action)
{
  sc_memory_arc_new(ctx, sc_type_arc_pos_const_perm, keynode_action_finished_successfully, action);
}

void finish_action_unsuccessfully(sc_memory_context * ctx, sc_addr action)
{
  sc_memory_arc_new(ctx, sc_type_arc_pos_const_perm, keynode_action_finished_unsuccessfully, action);
}
