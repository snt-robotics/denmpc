/**
 * @file    scenario.cpp
 * @Author  Jan Dentler (jan.dentler@uni.lu)
 *          University of Luxembourg
 * @date    27.February, 2017
 * @time    23:23h
 * @license GPLv3
 * @brief   User Scenario
 *
 * Scenario is defining containing int main() and defining the control scenario
 */

#include "Scheduler.h"
#include "Agent.h"
#include "Constraint.h"
#include "Coupling.h"

#include "Ardrone_20170227.h"
#include "OrientationConstraint_20170227.h"
#include "CollisionAvoidanceCoupling_20170227.h"

#include "Cmscgmres.h"
#include "Event.h"
#include "std_msgs/Bool.h"

int main(int argc, char **argv){

//Initialize ros node
ros::init(argc, argv, "controller");

/****** Initialize Agent Instances ******/
	std::vector<Agent*> agentlist;

	//Initialize: Ardrone instance: ardrone1
	Ardrone_20170227* ardrone0=new Ardrone_20170227(agentlist.size());

	double ardrone0_init_p[]={ //without tracking position
			-0.5092,1.458,-1,1,-5,1.3, //Model Parameters: Ardrone
			0.0,0.0,0.0, 0.0,0.0, 0.0,0.0,0.0,0.0, //State Penalty: no tracking
			1.0,1.0,0.3,1.0 //Input penalty
	};
	ardrone0->setInitialParameter(ardrone0_init_p);
	agentlist.push_back(ardrone0); /*add to agentlist*/

	//AddConstraint
	std::vector<Controller*> constraintlist;
	Constraint* constraint= new OrientationConstraint_20170227(ardrone0);
	//constraint_init_p{k0, ds, beta, ddist, kforw, kside, kup}
	double constraint0_init_p[]={1,0.17,0.5,1.5,1,1,1};
	constraint->setInitialParameter(constraint0_init_p);
	constraint->setParameter(constraint0_init_p);

/****** Initialize Coupling Instances ******/
	std::vector<Controller*> controllerlist;

	//Initialize: Controller
	Cmscgmres* controller1=new Cmscgmres(agentlist,controllerlist.size());
	controller1->activateInfo_ControllerStates();
	controllerlist.push_back(controller1);


/****** Initialize Events ******/
	std::vector<Event*> eventlist;

/****** Initialize Scheduler ******/
	Scheduler scheduler(argc,argv,controllerlist, eventlist);
	scheduler.run_control(0.01);
};



