#include <cstdio>
#include <iostream>
#include "Vec3.h"

using namespace std;



int main ()
{
	cout << "DEVELOPMENT HOMEWORK   vec3" << endl;
	cout << "-----------------------------------------------------------------------------------------" << endl;

	//Testing constructors
	vec3<float> vec1toTest (2,4,8);
	vec3<float> vec2toTest;
	vec3<float> vec3toTest(36,100,47);
	vec3<float> vec4toTest(vec1toTest);
	
	//Testing operators
	vec3<float> vec5toTest;
	vec5toTest = (vec1toTest + vec3toTest);
	vec3<float> vec6toTest;
	vec6toTest = vec1toTest - vec3toTest;


	vec3<float> vecArray[6]{vec1toTest,vec2toTest,vec3toTest,vec4toTest,vec5toTest,vec6toTest};
	
	// Start Printing the vectors/Results
	vecArray->printAllVecs(vecArray);
	/*cout << "Vector1: " << endl;
	vec1toTest.printVec();
	cout << "Vector2: " << endl;
	vec2toTest.printVec();
	cout << "Vector3: " << endl;
	vec3toTest.printVec();
	cout << "Vector4: " << endl;
	vec4toTest.printVec();
	cout << "Vector5: " << endl;
	vec5toTest.printVec();
	cout << "Vector6: " << endl;
	vec6toTest.printVec();*/

	bool vec1isZero = vec1toTest.is_zero();
	bool vec2isZero = vec2toTest.is_zero();
	
	cout << "Vec 1 Is zero: " << vec1isZero << endl;
	cout << "Vec 2 Is zero: " << vec2isZero << endl;

	if (vec1toTest == vec1toTest)
	{
		cout << "Both vectors ARE the same!" << endl;
	}
	else {
		cout << "These vectors AREN'T the same!" << endl;
	}
	system("pause");
	return 0;
}