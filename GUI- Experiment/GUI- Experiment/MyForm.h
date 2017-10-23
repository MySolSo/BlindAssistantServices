#pragma once
#include <string>
#include "C:\Python27\include\Python.h"
#include <atlstr.h>
#include <string.h>

namespace GUIExperiment {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Windows::Forms;
	using namespace System;
	using namespace System::Speech::Synthesis;
	using namespace std;
	using namespace System::Runtime::InteropServices;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{

	public:
		String^ fileName;
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}
		void textToSpeech(const std::string& textToBeRead)
		{
			System::String^ toRead = gcnew String(textToBeRead.c_str());
			SpeechSynthesizer^ synth = gcnew SpeechSynthesizer();

			synth->SelectVoiceByHints(VoiceGender::Female, VoiceAge::Adult);

			synth->Speak(toRead);
		}

		std::string callPythonOCR(char* fileName, char* functionName, const char* imagePath)
		{
			PyObject *pName, *pModule, *pDict, *pFunc, *pValue, *presult;


			// Initialize the Python Interpreter
			Py_Initialize();


			// Build the name object
			pName = PyString_FromString(fileName);

			// Load the module object
			pModule = PyImport_Import(pName);


			// pDict is a borrowed reference 
			pDict = PyModule_GetDict(pModule);


			// pFunc is also a borrowed reference 
			pFunc = PyDict_GetItemString(pDict, functionName);

			std::string text;

			if (PyCallable_Check(pFunc))
			{
				pValue = Py_BuildValue("(z)", imagePath);

				presult = PyObject_CallObject(pFunc, pValue);
				text = PyString_AsString(presult);

				Py_DECREF(pValue);
			}
			else
			{
				PyErr_Print();
			}

			// Clean up
			Py_DECREF(pModule);
			Py_DECREF(pName);

			// Finish the Python Interpreter
			Py_Finalize();

			textToSpeech(text);

			return text;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  button1;
	protected:
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::GroupBox^  Options;


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->Options = (gcnew System::Windows::Forms::GroupBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->Options->SuspendLayout();
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->button1->Location = System::Drawing::Point(11, 21);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 35);
			this->button1->TabIndex = 0;
			this->button1->Text = L"Open";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// pictureBox1
			// 
			this->pictureBox1->Dock = System::Windows::Forms::DockStyle::Left;
			this->pictureBox1->Location = System::Drawing::Point(0, 0);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(600, 600);
			this->pictureBox1->TabIndex = 1;
			this->pictureBox1->TabStop = false;
			// 
			// button2
			// 
			this->button2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->button2->Location = System::Drawing::Point(11, 75);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 36);
			this->button2->TabIndex = 2;
			this->button2->Text = L"Read";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
			// 
			// Options
			// 
			this->Options->Controls->Add(this->button1);
			this->Options->Controls->Add(this->button2);
			this->Options->Dock = System::Windows::Forms::DockStyle::Right;
			this->Options->Location = System::Drawing::Point(297, 0);
			this->Options->Name = L"Options";
			this->Options->Size = System::Drawing::Size(98, 312);
			this->Options->TabIndex = 3;
			this->Options->TabStop = false;
			this->Options->Text = L" ";
			this->Options->Enter += gcnew System::EventHandler(this, &MyForm::groupBox1_Enter);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(395, 312);
			this->Controls->Add(this->Options);
			this->Controls->Add(this->pictureBox1);
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->Options->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {

		OpenFileDialog^ sfd = gcnew OpenFileDialog();
		sfd->Filter = "Images|*.jpg|All Files|*.*";
		if (sfd->ShowDialog() != System::Windows::Forms::DialogResult::OK)
		{
			return;
		}
		fileName = sfd->FileName;
		//auto img = gcnew Bitmap(fileName);

		auto originalBitmap = gcnew Bitmap(this->pictureBox1->Width, this->pictureBox1->Height);
		this->pictureBox1->BackgroundImageLayout = ImageLayout::Stretch;
		this->pictureBox1->Image = gcnew Bitmap(fileName);
		//MessageBox::Show("OK");
	}
	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {

		const char* filePath = (const char*)(Marshal::StringToHGlobalAnsi(fileName)).ToPointer();
		MessageBox::Show(fileName);
		callPythonOCR("OCRFunc", "processImage", filePath);
	}
	private: System::Void groupBox1_Enter(System::Object^  sender, System::EventArgs^  e) {
	}
};
}
