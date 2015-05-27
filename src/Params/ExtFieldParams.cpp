#include "ExtFieldParams.h"

#include <cmath>

using namespace std;

ExtFieldParams::ExtFieldParams(PicParams& params, InputData &ifile) :
geometry(params.geometry)
{
	
    // -----------------
    // ExtFields properties
    // -----------------
    int n_extfield=0;
    while (ifile.existComponent("ext_field",n_extfield)) {
        
        ExtFieldStructure tmpExtField;
        ifile.extract("field",tmpExtField.fields,"ext_field",n_extfield);
        ifile.extract("profile",tmpExtField.profile,"ext_field",n_extfield);
        
        if (tmpExtField.profile.empty()) {
            PyObject *mypy = ifile.extract_py("profile","ext_field",n_extfield);
            if (mypy && PyCallable_Check(mypy)) {
                tmpExtField.py_profile=mypy;
                tmpExtField.profile="python";
            }
        } else {
            ifile.extract("int_params",tmpExtField.int_params,"ext_field",n_extfield);
            ifile.extract("double_params",tmpExtField.double_params,"ext_field",n_extfield);
            ifile.extract("length_params_x",tmpExtField.length_params_x,"ext_field",n_extfield);
            ifile.extract("length_params_y",tmpExtField.length_params_y,"ext_field",n_extfield);
            ifile.extract("length_params_z",tmpExtField.length_params_z,"ext_field",n_extfield);
            
            transform(tmpExtField.length_params_x.begin(), tmpExtField.length_params_x.end(), tmpExtField.length_params_x.begin(),
                      bind1st(multiplies<double>(),params.conv_fac));
            transform(tmpExtField.length_params_y.begin(), tmpExtField.length_params_y.end(), tmpExtField.length_params_y.begin(),
                      bind1st(multiplies<double>(),params.conv_fac));
            transform(tmpExtField.length_params_z.begin(), tmpExtField.length_params_z.end(), tmpExtField.length_params_z.begin(),
                      bind1st(multiplies<double>(),params.conv_fac));
        }   
        structs.push_back(tmpExtField);

        n_extfield++;
    }
    
}

