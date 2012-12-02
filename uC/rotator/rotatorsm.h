/*****************************************************************************
* Model: rotatorsm.qm
* File:  ./rotatorsm.h
*
* This file has been generated automatically by QP Modeler (QM).
* DO NOT EDIT THIS FILE MANUALLY.
*
* Please visit www.state-machine.com/qm for more information.
*****************************************************************************/
/* @(/1/0) .................................................................*/
typedef struct RotatorTag {
/* protected: */
    QActive super;

/* private: */
    int16_t current_heading;
    int16_t target_heading;
    int8_t rotate_dir;
    int8_t rot_idx;
    uint8_t dir_update_counter;
    RotatorError error;
    int16_t prev_heading;
    uint8_t wrong_dir_cnt;
    int16_t heading_scale;
    int16_t heading_offset;
    uint8_t stuck_cnt;
    uint16_t median_buf[3];
    uint8_t median_head;
    int8_t is_running;
} Rotator;

/* protected: */
QState Rotator_initial(Rotator *me);
QState Rotator_Idle(Rotator *me);
QState Rotator_Calibrate(Rotator *me);
QState Rotator_BreakReleased(Rotator *me);
QState Rotator_StartWait(Rotator *me);
QState Rotator_StopWait(Rotator *me);
QState Rotator_Running(Rotator *me);
QState Rotator_CCW(Rotator *me);
QState Rotator_CW(Rotator *me);
QState Rotator_Error(Rotator *me);

