#define PY_ARRAY_UNIQUE_SYMBOL pbcvt_ARRAY_API

#include <iostream>
#include <SeekCam.h>
#include <SeekThermal.h>
#include <Python.h>
#include <boost/python.hpp>
#include <opencv2/core/core.hpp>

#include <pyboostcvconverter/pyboostcvconverter.hpp>

namespace pbcvt {
    using namespace boost::python;

    class SeekThermalWrap: public LibSeek::SeekThermal,public wrapper<LibSeek::SeekThermal>
    {
        public:
            SeekThermalWrap(): LibSeek::SeekThermal(std::string()) 
            { }
            SeekThermalWrap(std::string ffc_filename): LibSeek::SeekThermal(ffc_filename) 
            { }
            bool open_wrap(void) 
            {
                return (this->open());
            }
            void close_wrap(void) 
            {
                return (this->close());
            }
            cv::Mat read_wrap(void) 
            {
                cv::Mat dst;

                if( !this->read(dst) )
                {
                    PyErr_SetString(PyExc_TypeError, "Failed to read frame from camera, exiting");
                }
                else 
                {
                    if( !dst.data )
                    {
                        PyErr_SetString(PyExc_TypeError, "Data frame is empty, exiting");
                    }
                }
                return(dst);
            }
    };

    //The following code is necessary to handle conversions b/w Python numpy arrays and OpenCV cv::Mat
#if (PY_VERSION_HEX >= 0x03000000)
    static void *init_ar() {
#else
    static void init_ar(){
#endif
        Py_Initialize();

        import_array();
        return NUMPY_IMPORT_ARRAY_RETVAL;
    }

    BOOST_PYTHON_MODULE(pylibseek_ext)
    {
        init_ar();
        //initialize converters
        to_python_converter<cv::Mat,
            pbcvt::matToNDArrayBoostConverter>();
        pbcvt::matFromNDArrayBoostConverter();

        class_<SeekThermalWrap,boost::noncopyable>("SeekThermal", init<optional<std::string> >())
            .def("open", &SeekThermalWrap::open_wrap)
            .def("close", &SeekThermalWrap::close_wrap)
            .def("read", &SeekThermalWrap::read_wrap);
    }
}
