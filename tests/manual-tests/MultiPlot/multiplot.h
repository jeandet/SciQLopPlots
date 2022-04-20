#ifndef SIMPLEPLOT_H
#define SIMPLEPLOT_H

#include <QMainWindow>
#include <SciQLopPlots/SciQLopPlot.hpp>
#include <SciQLopPlots/Qt/QCustomPlot/QCustomPlotWrapper.hpp>
#include "DataGenerator.hpp"
#include <SciQLopPlots/SyncPanel.hpp>
#include <QLabel>

class MultiPlot : public QMainWindow
{
    Q_OBJECT

public:
    MultiPlot(QWidget *parent = nullptr);
    ~MultiPlot();

private:
    SciQLopPlots::SciQLopPlot* makePlot(SciQLopPlots::SyncPannel* panel);
    std::vector<std::unique_ptr<DataProducer>> m_gens;
    QLabel* m_totalPointNumber;
};
#endif // SIMPLEPLOT_H
