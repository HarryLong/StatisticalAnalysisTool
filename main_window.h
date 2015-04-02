#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include "central_widget.h"
#include <QActionGroup>
#include "distribution_factory.h"
#include "dialogs.h"

class MainWindow : public QMainWindow
{
Q_OBJECT
public:
    MainWindow(QWidget * parent = 0, Qt::WindowFlags flags = 0);
    ~MainWindow();

protected:
    virtual void closeEvent ( QCloseEvent * event );

private slots:
    void refresh_input_widget_size();
    void show_random_distribution_dialog();
    void show_clustered_distribution_dialog();
    void generate_random_distribution();
    void generate_clustered_distribution();

private:
    void init_layout();
    void init_menu();
    void init_actions();

    QActionGroup * m_size_ag;

    int m_input_widget_width;
    int m_input_widget_height;

    // Menu elements
    QMenu * m_size_menu;
    QMenu * m_distributions_menu;
    QAction * m_generate_random_distribution_action;
    QAction * m_generate_clustered_action;

    CentralWidget * m_central_widget;
    RandomDistributionProducerDialog * m_random_distribution_producer_dlg;
    ClusteredDistributionProducerDialog * m_clustered_distribution_producer_dlg;

    DistributionFactory m_distribution_factory;
};

#endif //MAIN_WINDOW_H
