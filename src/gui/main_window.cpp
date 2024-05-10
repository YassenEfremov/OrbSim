#include "main_window.hpp"
#include "ui_main_window.h"

#include "simulation/integrators/euler.hpp"	
#include "simulation/integrators/verlet.hpp"	
#include "simulation/integrators/rk4.hpp"	
#include "simulation/math_obj.hpp"

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>

#include <functional>
#include <map>


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), ui(new Ui::MainWindow) {

	ui->setupUi(this);

	connect(ui->SimulateButton, &QPushButton::clicked,
			this, &MainWindow::simulate);
	connect(ui->ExampleValuesButton, &QPushButton::clicked,
			this, &MainWindow::load_example_values);
}

MainWindow::~MainWindow() {
	delete ui;
}

void MainWindow::simulate() {

	// Initial conditions

	double M = 5.972e24;	// Earth Mass in [kg]
	double R0 = 6378.137;	// Earth Radius in [km]

	orbsim::Vec3 x0{
		ui->PosSpinBoxX->value(),
		ui->PosSpinBoxY->value(),
		ui->PosSpinBoxZ->value()
	};
	orbsim::Vec3 v0{
		ui->VelSpinBoxX->value(),
		ui->VelSpinBoxY->value(),
		ui->VelSpinBoxZ->value()
	};

	double t_i = ui->StartTimeSpinBox->value();
	double t_f = ui->FinishTimeSpinBox->value();

	std::map<QString, std::function<orbsim::Integrator *()>> str_integ {
		{"Euler", [=]() { return new orbsim::Euler(M, R0, x0, v0, t_i, t_f, t_f/10); }},
		{"Verlet", [=]() { return new orbsim::Verlet(M, R0, x0, v0, t_i, t_f, t_f/10); }},
		{"RK4", [=]() { return new orbsim::RK4(M, R0, x0, v0, t_i, t_f, t_f/10); }}
	};

	orbsim::Integrator *integ = str_integ[ui->ChooseIntegrator->currentText()]();
	integ->integrate();

	QString output;
	for (int i = 0; i < integ->get_steps() - 1; i++) {
		output += integ->get_pos_arr()[i].to_str() + " " + integ->get_vel_arr()[i].to_str() + "\n";
	}

	delete integ;

	ui->OutputConsole->setText(output);
}

void MainWindow::load_example_values() {

	// Example initial conditions

	double M = 5.972e24;	// Earth Mass in [kg]
	double R0 = 6378.137;	// Earth Radius in [km]

	orbsim::Vec3 x0{7000, 0.000001, -0.001608};
	orbsim::Vec3 v0{0.000002, 1.310359, 7.431412};

	double t_i = 0;
	double t_f = 86400;

	ui->PosSpinBoxX->setValue(x0.x);
	ui->PosSpinBoxY->setValue(x0.y);
	ui->PosSpinBoxZ->setValue(x0.z);

	ui->VelSpinBoxX->setValue(v0.x);
	ui->VelSpinBoxY->setValue(v0.y);
	ui->VelSpinBoxZ->setValue(v0.z);

	ui->StartTimeSpinBox->setValue(t_i);
	ui->FinishTimeSpinBox->setValue(t_f);
	ui->TimeStepSpinBox->setValue(t_f/10);
}
