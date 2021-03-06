#include "DataHandler.h"
#pragma region JSON_Methods
//����������� ��� ���������������� ��� ��� ���������� json ��� ������� parsing ������
//custom ������� ��� ��������� json. � ���������� ��� ��������� �� ����� ���� ���� ������,
//������ �� ����� ��� ���� namespace ���� ������� � ����� ����.

void to_json(json& j, const Appointment& appointment) {
	j = json{
		{ "doctorId", appointment.getDoctorId() },
		{ "patientId", appointment.getPatientId() },
		{ "code", appointment.getCode() },
	};
}
void to_json(json& j, const Appointment* appointment) {
	j = json{
		{ "doctorId", appointment->getDoctorId() },
		{ "patientId", appointment->getPatientId() },
		{ "code", appointment->getCode() },
	};
}

void from_json(const json& j, Appointment& appointment) {
	appointment = Appointment(
		j.at("code").get<string>(),
		j.at("doctorId").get<string>(),
		j.at("patientId").get<string>()
	);
}

void to_json(json& j, const DailySchedule::timeSlot& appointment) {
	j = json{
		{ "appointment", appointment.appointment },
		{ "date" , appointment.date },
		{ "time" , appointment.time }
	};
}
void to_json(json& j, const DailySchedule::timeSlot* appointment) {
	j = json{
		{ "appointment", appointment->appointment },
		{ "date" , appointment->date },
		{ "time" , appointment->time }
	};
}
void from_json(const json& j, DailySchedule::timeSlot& appointment) {
	appointment.appointment = j.at("appointment").get<Appointment>();
	appointment.date = j.at("date").get<string>();
	appointment.time = j.at("time").get<string>();
}

void to_json(json& j, const DailySchedule& dailySchedule) {
	j = json{
		{ "appointments", dailySchedule.getAppointments() }
	};
}
void to_json(json& j, const DailySchedule* dailySchedule) {
	j = json{
		{ "appointments", dailySchedule->getAppointments() }
	};
}

void from_json(const json& j, DailySchedule& dailySchedule) {
	dailySchedule.setAppointments(
		j.at("appointments").get<vector<DailySchedule::timeSlot>>()
	);
}

void to_json(json& j, const Patient& patient) {
	j = json{
		{ "code", patient.getCode() },
		{ "name", patient.getName() },
		{ "age", patient.getAge() },
		{ "schedule", patient.getSchedule() }
	};
}

void to_json(json& j, const Patient* patient) {
	j = json{
		{ "code", patient->getCode() },
		{ "name", patient->getName() },
		{ "age", patient->getAge() },
		{ "schedule", patient->getSchedule() }
	};
}

//tempPatient ����� ���������������� �� strings ���� ������ ��� �� block.
Patient tempPatient;
void from_json(const json& j, Patient& patient) {
	patient.setCode(j.at("code").get<string>());
	patient.setName(j.at("name").get<string>());
	patient.setAge(j.at("age").get<int>());
	patient.setSchedule(j.at("schedule").get<DailySchedule>());
	tempPatient = patient;
}

void to_json(json& j, const Doctor* doctor) {
	j = json{
		{ "code", doctor->getCode() },
		{ "name", doctor->getName() },
		{ "age", doctor->getAge() },
		{ "specialty", doctor->getSpecialty() },
		{ "schedule", doctor->getSchedule() }
	};
}

void to_json(json& j, const Doctor& doctor) {
	j = json{
		{ "code", doctor.getCode() },
		{ "name", doctor.getName() },
		{ "age", doctor.getAge() },
		{ "specialty", doctor.getSpecialty() },
		{ "schedule", doctor.getSchedule() }
	};
}

//tempDoctor ����� ���������������� �� strings ���� ������ ��� �� block.
Doctor tempDoctor;
void from_json(const json& j, Doctor& doctor) {
	doctor.setCode(j.at("code").get<string>());
	doctor.setName(j.at("name").get<string>());
	doctor.setAge(j.at("age").get<int>());
	doctor.setSpecialty(j.at("specialty").get<string>());
	doctor.setSchedule(j.at("schedule").get<DailySchedule>());
	tempDoctor = doctor;
}
#pragma endregion
		

DataHandler::DataHandler(vector<Doctor*> *doctors, vector<Patient*> *patients) {
	fstream fs;
	json scheduledData;

	fs.open("C:/Users/demos/Desktop/scheduledData.json", fstream::in);
	if (!fs.fail()) {
		fs >> scheduledData;
		fs.close();

		json doctorsJ = scheduledData.find("doctors").value();
		json patientsJ = scheduledData.find("patients").value();

		for (json::iterator doctor = doctorsJ.begin(); doctor != doctorsJ.end(); ++doctor) {
			json newDoctorJ = doctor.value();
			newDoctorJ.get<Doctor>();
			doctors->push_back(new Doctor(tempDoctor));
		}

		for (json::iterator patient = patientsJ.begin(); patient != patientsJ.end(); ++patient) {
			json newPatientJ = patient.value();
			newPatientJ.get<Patient>();
			patients->push_back(new Patient(tempPatient));
		}
	}
}

void DataHandler::save(vector<Doctor*> *doctors, vector<Patient*> *patients) {
	fstream fs;
	json scheduledData;

	fs.open("scheduledData.json", fstream::out);
	if (!fs.fail()) {
		
		json jDoctors = *doctors;
		json jPatients = *patients;
		scheduledData["doctors"] = jDoctors;
		scheduledData["patients"] = jPatients;

		fs << scheduledData;
		fs.close();
	}
}