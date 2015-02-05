#ifndef TEMPSETTINGS_H
#define TEMPSETTINGS_H

#include "graphicscontrol/controlbaseclass.h"
#include <vector>

/*!
 * \brief The TempSettings class is just a placeholder for the main controller class? or just the
 * game settings class.
 */
class TempSettings {
    public:

        unsigned int window_width;
        unsigned int window_height;

        double mapw;
        double maph;
        double mapx;
        double mapy;
        double mapmidx;
        double mapmidy;

        /*!
         * \brief A list of other controllers that are affected by a change within this.
         */
        std::vector<ControlBaseClass*> affectedsettings;

        void addToList(ControlBaseClass* newcontrol){
            affectedsettings.push_back(newcontrol);
        }

        void when_a_setting_is_changed(){
            // change the setting
            // eg. mapw = 1400

            for (unsigned int i = 0; i < affectedsettings.size(); ++i)
                affectedsettings[i]->update_settings();
        }

        TempSettings();

        ~TempSettings(){}

};

#endif // TEMPSETTINGS_H
