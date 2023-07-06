/**
 * @brief Fichero generado por SquareLine Studio 1.2.3
 * @file ui_comp.h
 * @author Jesús Carmelo González Domínguez
 * @date 14-07-2023
*/

#ifndef _SQUARELINE_PROJECT_UI_COMP_H
#define _SQUARELINE_PROJECT_UI_COMP_H

#ifdef __cplusplus
extern "C" {
#endif


lv_obj_t * ui_comp_get_child(lv_obj_t *comp, uint32_t child_idx);
extern uint32_t LV_EVENT_GET_COMP_CHILD;

// COMPONENT Label1
#define UI_COMP_LABEL1_LABEL1 0
#define _UI_COMP_LABEL1_NUM 1
lv_obj_t *ui_Label1_create(lv_obj_t *comp_parent);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
