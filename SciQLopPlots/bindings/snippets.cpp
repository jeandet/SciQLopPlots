// @snippet QCPGraph-setData
NpArray_view x{pyArgs[0]};
NpArray_view y{pyArgs[1]};
QVector<QCPGraphData> data(x.flat_size());
const auto x_data = x.data();
const auto y_data = y.data();
for(auto i=0UL;i<x.flat_size();i++)
{
    data[i]={x_data[i],y_data[i]};
}
%CPPSELF.data()->set(std::move(data),true);
// @snippet QCPGraph-setData


// @snippet QCPColorMapData-cellToCoord
double a=0;
double b=0;
%CPPSELF.%FUNCTION_NAME (%1,%2,&a,&b);
%PYARG_0 = PyTuple_New(2);
PyTuple_SET_ITEM(%PYARG_0, 0, %CONVERTTOPYTHON[double](a));
PyTuple_SET_ITEM(%PYARG_0, 1, %CONVERTTOPYTHON[double](b));
// @snippet QCPColorMapData-cellToCoord

// @snippet QCPColorMapData-coordToCell
int a=0;
int b=0;
%CPPSELF.%FUNCTION_NAME (%1,%2,&a,&b);
%PYARG_0 = PyTuple_New(2);
PyTuple_SET_ITEM(%PYARG_0, 0, %CONVERTTOPYTHON[int](a));
PyTuple_SET_ITEM(%PYARG_0, 1, %CONVERTTOPYTHON[int](b));
// @snippet QCPColorMapData-coordToCell

// @snippet QCustomPlot-QCPColorMap
auto r = new QCPColorMap{%1,%2};
pyResult = %CONVERTTOPYTHON[QCPColorMap](*r);
// @snippet QCustomPlot-QCPColorMap
