/*** 243 thunderbird - compass ****/


#ifndef COMPASS_HPP_
#define COMPASS_HPP_

#include "singleton_template.hpp"
#include "i2c2_device.hpp"  // I2C Device base class

typedef struct compass_data{
        float yaw;
        float pitch;
        float roll;
}compass_data_t;


#if 0
class I2C_comp : private i2c2_device
{
    public:
        I2C_comp(char addr): i2c2_device(addr){}   //define all compass function calls here
        bool init_compass();

#if 1
        void get_compass_data();
#else if
        bool loop();
#endif
};

class compass:public I2C_comp , public SingletonTemplate<compass>
{
            public:
                /* API is at I2C_comp */

            private:
                /// Private constructor of this Singleton class
                compass() :  I2C_comp(I2CAddr_Compass)
                {
                }

                friend class SingletonTemplate<compass>;  ///< Friend class used for Singleton Template
        };

*/
        #endif /* COMPASS_HPP_ */
#endif
