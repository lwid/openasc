/*****************************************************************************
* Model: rotatorsm.qm
* File:  ./rotatorsm.c
*
* This file has been generated automatically by QP Modeler (QM).
* DO NOT EDIT THIS FILE MANUALLY.
*
* Please visit www.state-machine.com/qm for more information.
*****************************************************************************/
/* @(/1/0) .................................................................*/
/* @(/1/0/14) ..............................................................*/
/* @(/1/0/14/0) */
QState Rotator_initial(Rotator *me) {
    return Q_TRAN(&Rotator_Idle);
}
/* @(/1/0/14/1) ............................................................*/
QState Rotator_Idle(Rotator *me) {
    switch (Q_SIG(me)) {
        /* @(/1/0/14/1) */
        case Q_ENTRY_SIG: {
            DEBUG_PRINT("Idle: ENTER\n");
            me->rotate_dir = 0;
            return Q_HANDLED();
        }
        /* @(/1/0/14/1) */
        case Q_EXIT_SIG: {
            DEBUG_PRINT("Idle: EXIT\n");
            return Q_HANDLED();
        }
        /* @(/1/0/14/1/0) */
        case CAL_ENABLE_SIG: {
            return Q_TRAN(&Rotator_Calibrate);
        }
        /* @(/1/0/14/1/1) */
        case ROTATE_CW_SIG: {
            /* @(/1/0/14/1/1/1) */
            if (cw_limit_exceeded(me)) {
                return Q_TRAN(&Rotator_Idle);
            }
            /* @(/1/0/14/1/1/0) */
            else {
                me->rotate_dir = 1;
                return Q_TRAN(&Rotator_BreakReleased);
            }
        }
        /* @(/1/0/14/1/2) */
        case ROTATE_CCW_SIG: {
            /* @(/1/0/14/1/2/1) */
            if (ccw_limit_exceeded(me)) {
                return Q_TRAN(&Rotator_Idle);
            }
            /* @(/1/0/14/1/2/0) */
            else {
                me->rotate_dir = -1;
                return Q_TRAN(&Rotator_BreakReleased);
            }
        }
        /* @(/1/0/14/1/3) */
        case ROTATOR_ERROR_SIG: {
            return Q_TRAN(&Rotator_Error);
        }
        /* @(/1/0/14/1/4) */
        case Q_INIT_SIG: {
            if (!is_calibrated(me)) {
              me->error = ROTATOR_ERROR_NO_CAL;
              QActive_post((QActive *)me, ROTATOR_ERROR_SIG, 0);
            }
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&QHsm_top);
}
/* @(/1/0/14/2) ............................................................*/
QState Rotator_Calibrate(Rotator *me) {
    switch (Q_SIG(me)) {
        /* @(/1/0/14/2) */
        case Q_ENTRY_SIG: {
            DEBUG_PRINT("Calibrate: ENTER\n");
            //bsp_rotator_release_break(me->rot_idx);
            return Q_HANDLED();
        }
        /* @(/1/0/14/2) */
        case Q_EXIT_SIG: {
            DEBUG_PRINT("Calibrate: EXIT\n");
            bsp_rotator_stop(me->rot_idx);
            //bsp_rotator_apply_break(me->rot_idx);
            me->error = 0;
            Rotator_calc_heading_coeffs(me);
            eeprom_write_config();
            return Q_HANDLED();
        }
        /* @(/1/0/14/2/0) */
        case CAL_DISABLE_SIG: {
            return Q_TRAN(&Rotator_Idle);
        }
        /* @(/1/0/14/2/1) */
        case ROTATE_CW_SIG: {
            bsp_rotator_run_cw(me->rot_idx);
            return Q_HANDLED();
        }
        /* @(/1/0/14/2/2) */
        case ROTATE_CCW_SIG: {
            bsp_rotator_run_ccw(me->rot_idx);
            return Q_HANDLED();
        }
        /* @(/1/0/14/2/3) */
        case STOP_SIG: {
            bsp_rotator_stop(me->rot_idx);
            return Q_HANDLED();
        }
        /* @(/1/0/14/2/4) */
        case SET_CCW_LIMIT_SIG: {
            Rotator_set_ccw_limit(me, Q_PAR(me));
            return Q_HANDLED();
        }
        /* @(/1/0/14/2/5) */
        case SET_CW_LIMIT_SIG: {
            Rotator_set_cw_limit(me, Q_PAR(me));
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&QHsm_top);
}
/* @(/1/0/14/3) ............................................................*/
QState Rotator_BreakReleased(Rotator *me) {
    switch (Q_SIG(me)) {
        /* @(/1/0/14/3) */
        case Q_ENTRY_SIG: {
            DEBUG_PRINT("BreakReleased: ENTER\n");
            bsp_rotator_release_break(me->rot_idx);
            return Q_HANDLED();
        }
        /* @(/1/0/14/3) */
        case Q_EXIT_SIG: {
            DEBUG_PRINT("BreakReleased: EXIT\n");
            me->rotate_dir = 0;
            bsp_rotator_apply_break(me->rot_idx);
            return Q_HANDLED();
        }
        /* @(/1/0/14/3/0) */
        case Q_INIT_SIG: {
            return Q_TRAN(&Rotator_StartWait);
        }
        /* @(/1/0/14/3/1) */
        case ROTATE_CW_SIG: {
            Rotator_set_rotate_dir(me, 1);
            return Q_HANDLED();
        }
        /* @(/1/0/14/3/2) */
        case ROTATE_CCW_SIG: {
            Rotator_set_rotate_dir(me, -1);
            return Q_HANDLED();
        }
        /* @(/1/0/14/3/3) */
        case ROTATOR_ERROR_SIG: {
            Rotator_set_rotate_dir(me, 0);
            return Q_TRAN(&Rotator_StopWait);
        }
        /* @(/1/0/14/3/4) */
        case STOP_SIG: {
            Rotator_set_rotate_dir(me, 0);
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&QHsm_top);
}
/* @(/1/0/14/3/5) ..........................................................*/
QState Rotator_StartWait(Rotator *me) {
    switch (Q_SIG(me)) {
        /* @(/1/0/14/3/5) */
        case Q_ENTRY_SIG: {
            DEBUG_PRINT("StartWait: ENTER\n");
            QActive_arm((QActive *)me, 10);
            return Q_HANDLED();
        }
        /* @(/1/0/14/3/5) */
        case Q_EXIT_SIG: {
            DEBUG_PRINT("StartWait: EXIT\n");
            return Q_HANDLED();
        }
        /* @(/1/0/14/3/5/0) */
        case Q_TIMEOUT_SIG: {
            return Q_TRAN(&Rotator_Running);
        }
        /* @(/1/0/14/3/5/1) */
        case STOP_SIG: {
            return Q_TRAN(&Rotator_Idle);
        }
    }
    return Q_SUPER(&Rotator_BreakReleased);
}
/* @(/1/0/14/3/6) ..........................................................*/
QState Rotator_StopWait(Rotator *me) {
    switch (Q_SIG(me)) {
        /* @(/1/0/14/3/6) */
        case Q_ENTRY_SIG: {
            DEBUG_PRINT("StopWait: ENTER\n");
            QActive_arm((QActive *)me, 30);
            return Q_HANDLED();
        }
        /* @(/1/0/14/3/6) */
        case Q_EXIT_SIG: {
            DEBUG_PRINT("StopWait: EXIT\n");
            return Q_HANDLED();
        }
        /* @(/1/0/14/3/6/0) */
        case Q_TIMEOUT_SIG: {
            /* @(/1/0/14/3/6/0/0) */
            if (me->error != ROTATOR_ERROR_OK) {
                return Q_TRAN(&Rotator_Error);
            }
            /* @(/1/0/14/3/6/0/1) */
            else {
                /* @(/1/0/14/3/6/0/1/1) */
                if (me->rotate_dir == 0) {
                    return Q_TRAN(&Rotator_Idle);
                }
                /* @(/1/0/14/3/6/0/1/0) */
                else {
                    return Q_TRAN(&Rotator_Running);
                }
            }
        }
    }
    return Q_SUPER(&Rotator_BreakReleased);
}
/* @(/1/0/14/3/7) ..........................................................*/
QState Rotator_Running(Rotator *me) {
    switch (Q_SIG(me)) {
        /* @(/1/0/14/3/7) */
        case Q_ENTRY_SIG: {
            DEBUG_PRINT("Running: ENTER\n");
            Q_REQUIRE(me->rotate_dir != 0);
            me->is_running = 1;
            rotator_direction_updated(me->rot_idx, Rotator_adc2deg(me, me->current_heading));
            return Q_HANDLED();
        }
        /* @(/1/0/14/3/7) */
        case Q_EXIT_SIG: {
            DEBUG_PRINT("Running: EXIT\n");
            bsp_rotator_stop(me->rot_idx);
            me->is_running = 0;
            rotator_direction_updated(me->rot_idx, Rotator_adc2deg(me, me->current_heading));
            return Q_HANDLED();
        }
        /* @(/1/0/14/3/7/0) */
        case Q_INIT_SIG: {
            /* @(/1/0/14/3/7/0/1) */
            if (me->rotate_dir > 0) {
                return Q_TRAN(&Rotator_CW);
            }
            /* @(/1/0/14/3/7/0/0) */
            else {
                return Q_TRAN(&Rotator_CCW);
            }
        }
        /* @(/1/0/14/3/7/1) */
        case STOP_SIG: {
            Rotator_set_rotate_dir(me, 0);
            return Q_TRAN(&Rotator_StopWait);
        }
        /* @(/1/0/14/3/7/2) */
        case ROTATION_LIMIT_SIG: {
            Rotator_set_rotate_dir(me, 0);
            return Q_TRAN(&Rotator_StopWait);
        }
    }
    return Q_SUPER(&Rotator_BreakReleased);
}
/* @(/1/0/14/3/7/3) ........................................................*/
QState Rotator_CCW(Rotator *me) {
    switch (Q_SIG(me)) {
        /* @(/1/0/14/3/7/3) */
        case Q_ENTRY_SIG: {
            DEBUG_PRINT("CCW: ENTER\n");
            bsp_rotator_run_ccw(me->rot_idx);
            return Q_HANDLED();
        }
        /* @(/1/0/14/3/7/3) */
        case Q_EXIT_SIG: {
            DEBUG_PRINT("CCW: EXIT\n");
            return Q_HANDLED();
        }
        /* @(/1/0/14/3/7/3/0) */
        case ROTATE_CCW_SIG: {
            return Q_HANDLED();
        }
        /* @(/1/0/14/3/7/3/1) */
        case ROTATE_CW_SIG: {
            Rotator_set_rotate_dir(me, 1);
            return Q_TRAN(&Rotator_StopWait);
        }
    }
    return Q_SUPER(&Rotator_Running);
}
/* @(/1/0/14/3/7/4) ........................................................*/
QState Rotator_CW(Rotator *me) {
    switch (Q_SIG(me)) {
        /* @(/1/0/14/3/7/4) */
        case Q_ENTRY_SIG: {
            DEBUG_PRINT("CW: ENTER\n");
            bsp_rotator_run_cw(me->rot_idx);
            return Q_HANDLED();
        }
        /* @(/1/0/14/3/7/4) */
        case Q_EXIT_SIG: {
            DEBUG_PRINT("CW: EXIT\n");
            return Q_HANDLED();
        }
        /* @(/1/0/14/3/7/4/0) */
        case ROTATE_CW_SIG: {
            return Q_HANDLED();
        }
        /* @(/1/0/14/3/7/4/1) */
        case ROTATE_CCW_SIG: {
            Rotator_set_rotate_dir(me, -1);
            return Q_TRAN(&Rotator_StopWait);
        }
    }
    return Q_SUPER(&Rotator_Running);
}
/* @(/1/0/14/4) ............................................................*/
QState Rotator_Error(Rotator *me) {
    switch (Q_SIG(me)) {
        /* @(/1/0/14/4) */
        case Q_ENTRY_SIG: {
            DEBUG_PRINT("Error: ENTER\n");
            me->rotate_dir = 0;
            rotator_error(me->rot_idx, me->error);
            return Q_HANDLED();
        }
        /* @(/1/0/14/4) */
        case Q_EXIT_SIG: {
            DEBUG_PRINT("Error: EXIT\n");
            me->rotate_dir = 0;
            me->error = ROTATOR_ERROR_OK;
            rotator_error(me->rot_idx, me->error);
            return Q_HANDLED();
        }
        /* @(/1/0/14/4/0) */
        case ROTATE_CW_SIG: {
            rotator_error(me->rot_idx, me->error);
            if (++me->rotate_dir == 2) {
              QActive_arm((QActive *)me, 30);
            }
            return Q_HANDLED();
        }
        /* @(/1/0/14/4/1) */
        case ROTATE_CCW_SIG: {
            rotator_error(me->rot_idx, me->error);
            if (++me->rotate_dir == 2) {
              QActive_arm((QActive *)me, 30);
            }
            return Q_HANDLED();
        }
        /* @(/1/0/14/4/2) */
        case Q_TIMEOUT_SIG: {
            return Q_TRAN(&Rotator_Idle);
        }
        /* @(/1/0/14/4/3) */
        case STOP_SIG: {
            me->rotate_dir = 0;
            QActive_disarm((QActive *)me);
            return Q_HANDLED();
        }
        /* @(/1/0/14/4/4) */
        case CAL_ENABLE_SIG: {
            return Q_TRAN(&Rotator_Calibrate);
        }
    }
    return Q_SUPER(&QHsm_top);
}

