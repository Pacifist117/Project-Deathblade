#ifndef CONTROLBASECLASS_H
#define CONTROLBASECLASS_H

/*!
 * \brief The ControlBaseClass class is the template for what every controller class will have.
 *
 * As of Feb 02, 2015 my idea is to have controller classes that control the main processes of the program.
 * For example, the CameraControlClass keeps track of the camera and controls the view of the user. Each of
 * the control classes will inherit ControlBaseClass so it will have access to any settings changed that affect
 * it.
 */
class ControlBaseClass {

public:
    ControlBaseClass();
    virtual ~ControlBaseClass(){}

    /*!
     * \brief Recalculates all inner members that are dependent on other settings. This function
     * should be called for every controller class when a setting is changed.
     */
    virtual void update_settings(){}
};

#endif // CONTROLBASECLASS_H
