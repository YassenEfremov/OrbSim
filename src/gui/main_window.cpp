#include "main_window.hpp"
#include "ui_main_window.h"

#include "simulation/integrators/euler.hpp"	
#include "simulation/integrators/verlet.hpp"	
#include "simulation/integrators/rk4.hpp"	
#include "simulation/math_obj.hpp"
#include "simulation/satellite.hpp"

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

	double ecc = ui->EccSpinBox->value();
	double sem_maj_ax = ui->SemMajAxSpinBox->value();
	double inc = ui->IncSpinBox->value();
	double ri_asc_node = ui->RiAscNodeSpinBox->value();
	double arg_of_per = ui->ArgOfPerSpinBox->value();
	double true_anom = ui->TrueAnomSpinBox->value();

	double t_i = ui->StartTimeSpinBox->value();
	double t_f = ui->FinishTimeSpinBox->value();
	double t_step = t_f/10;

	std::map<QString, std::function<orbsim::Integrator *()>> str_integ {
		{"Euler", [=]() { return new orbsim::Euler(orbsim::M_Earth, orbsim::R_Earth, x0, v0, t_i, t_f, t_step); }},
		{"Verlet", [=]() { return new orbsim::Verlet(orbsim::M_Earth, orbsim::R_Earth, x0, v0, t_i, t_f, t_step); }},
		{"RK4", [=]() { return new orbsim::RK4(orbsim::M_Earth, orbsim::R_Earth, x0, v0, t_i, t_f, t_step); }}
	};

	orbsim::Integrator *integ = str_integ[ui->ChooseIntegrator->currentText()]();
	integ->integrate();

	// just for testing
	orbsim::Satellite sat1(x0, v0);
	orbsim::Satellite sat2(
		ecc, sem_maj_ax,
		inc, ri_asc_node,
		arg_of_per, true_anom
	);

	QString output;
	for (int i = 0; i < integ->get_steps() - 1; i++) {
		output += integ->get_pos_arr()[i].to_str() + " " + integ->get_vel_arr()[i].to_str() + "\n";
	}

	delete integ;

	ui->OutputConsole->setText(output);
}

void MainWindow::load_example_values() {

	// Example initial conditions

	orbsim::Vec3 x0{7000, 0.000001, -0.001608};
	orbsim::Vec3 v0{0.000002, 1.310359, 7.431412};

	double ecc = 2.82977289672605e-05;
	double sem_maj_ax = 7000.19808952185;
	double inc = 1.396263328559;
	double ri_asc_node = 4.2146848510894e-08;
	double arg_of_per = 6.28181250052071;
	double true_anom = 	0.00137257340076574;

	double t_i = 0;
	double t_f = 86400;
	double t_step = t_f/10;

	ui->PosSpinBoxX->setValue(x0.x);
	ui->PosSpinBoxY->setValue(x0.y);
	ui->PosSpinBoxZ->setValue(x0.z);

	ui->VelSpinBoxX->setValue(v0.x);
	ui->VelSpinBoxY->setValue(v0.y);
	ui->VelSpinBoxZ->setValue(v0.z);

	ui->StartTimeSpinBox->setValue(t_i);
	ui->FinishTimeSpinBox->setValue(t_f);
	ui->TimeStepSpinBox->setValue(t_step);

	ui->EccSpinBox->setValue(ecc);
	ui->SemMajAxSpinBox->setValue(sem_maj_ax);
	ui->IncSpinBox->setValue(inc);
	ui->RiAscNodeSpinBox->setValue(ri_asc_node);
	ui->ArgOfPerSpinBox->setValue(arg_of_per);
	ui->TrueAnomSpinBox->setValue(true_anom);
}
