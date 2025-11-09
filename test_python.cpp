#include <Python.h>
#include <iostream>

int main() {
    std::cout << "Initializing Python..." << std::endl;
    Py_Initialize();

    if (!Py_IsInitialized()) {
        std::cerr << "Failed to initialize Python" << std::endl;
        return -1;
    }

    std::cout << "Adding current directory to sys.path..." << std::endl;
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.insert(0, '.')");

    std::cout << "Importing fuzzy_module..." << std::endl;
    PyObject* pName = PyUnicode_DecodeFSDefault("fuzzy_module");
    PyObject* pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (!pModule) {
        std::cerr << "Failed to import module" << std::endl;
        PyErr_Print();
        return -1;
    }

    std::cout << "Getting compute_quality function..." << std::endl;
    PyObject* pFunc = PyObject_GetAttrString(pModule, "compute_quality");

    if (!pFunc || !PyCallable_Check(pFunc)) {
        std::cerr << "Function not found or not callable" << std::endl;
        PyErr_Print();
        return -1;
    }

    std::cout << "Calling compute_quality(60, 60, 50, 50, 50)..." << std::endl;
    PyObject* pArgs = PyTuple_New(5);
    PyTuple_SetItem(pArgs, 0, PyFloat_FromDouble(60.0));
    PyTuple_SetItem(pArgs, 1, PyFloat_FromDouble(60.0));
    PyTuple_SetItem(pArgs, 2, PyFloat_FromDouble(50.0));
    PyTuple_SetItem(pArgs, 3, PyFloat_FromDouble(50.0));
    PyTuple_SetItem(pArgs, 4, PyFloat_FromDouble(50.0));

    PyObject* pResult = PyObject_CallObject(pFunc, pArgs);
    Py_DECREF(pArgs);

    if (!pResult) {
        std::cerr << "Function call failed" << std::endl;
        PyErr_Print();
        return -1;
    }

    long result = PyLong_AsLong(pResult);
    Py_DECREF(pResult);

    std::cout << "Result: " << result << std::endl;
    std::cout << "SUCCESS!" << std::endl;

    Py_XDECREF(pFunc);
    Py_DECREF(pModule);
    Py_Finalize();

    return 0;
}