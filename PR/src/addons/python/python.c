#include <Python.h>
#include "pr.h"

/* Implemented functions:
 * 
 * prClassifyDat
 * prExportDat
 * prExportDatOpt
 * prImportDat
 * prImportDatOpt
 * prSetOutput
 * setCompressionLevel
 */

/* setCompressionLevel */
static PyObject* pr_setCompressionLevel(PyObject *self, PyObject *args){
	int cl;

	if (!PyArg_ParseTuple(args, "i", &cl)) return NULL;
	setCompressionLevel(cl);
	return Py_BuildValue("i", 0);
}

/* prClassifyDat */
static PyObject* pr_classifyDat(PyObject *self, PyObject *args){
	char* vFiledat;
	int result;

	if (!PyArg_ParseTuple(args, "s", &vFiledat)) return NULL;
	result=prClassifyDat(vFiledat);
	return Py_BuildValue("i", result);
}

/* prExportDat */
static PyObject* pr_exportDat(PyObject *self, PyObject *args){
	char* vDatFile;
	char* vDirName;
	char* vResFile;
	int result;

	if (!PyArg_ParseTuple(args, "sss", &vDatFile,&vDirName,&vResFile)) return NULL;
	result=prExportDat(vDatFile,vDirName,vResFile);
	return Py_BuildValue("i", result);
}

/* prImportDat */
static PyObject* pr_importDat(PyObject *self, PyObject *args){
	char* vDatFile;
	char* vDirName;
	char* vResFile;
	int result;

	if (!PyArg_ParseTuple(args, "sss", &vDatFile,&vDirName,&vResFile)) return NULL;
	result=prImportDat(vDatFile,vDirName,vResFile);
	return Py_BuildValue("i", result);
}

/* Python initialization */
static PyMethodDef SpamMethods[] = {
    {"setCompressionLevel",  pr_setCompressionLevel, METH_VARARGS,
     "Sets the compression level to be used by the module."},
    {"classifyDat",  pr_classifyDat, METH_VARARGS,
     "Checks if a file is a valid POP DAT file and tries to return the DAT file type."},
    {"exportDat",  pr_exportDat, METH_VARARGS,
     "Exports files from a given DAT archive."},
    {"importDat",  pr_importDat, METH_VARARGS,
     "Imports files to a given DAT archive."},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC initpr(void) {
    (void) Py_InitModule("pr", SpamMethods);
}

int main(int argc, char *argv[]) {
	/* Pass argv[0] to the Python interpreter */
	Py_SetProgramName(argv[0]);

	/* Initialize the Python interpreter.  Required. */
	Py_Initialize();

	/* Add a static module */
	initpr();
	return 0;
}

