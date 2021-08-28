#include "data_upload.h"


#define DATA_UPLOAD(...)   Serial.printf(__VA_ARGS__)




void vData_Upload_task(void * parameter)
{
    float i=0;
    float a,b;
    while (1)
    {
        a = sin(i)*100;
        b = cos(i++)*100;
      //  DATA_UPLOAD("A,%f;B,%f\n",a,b);
        vTaskDelay(20);
      
    }
    
}



