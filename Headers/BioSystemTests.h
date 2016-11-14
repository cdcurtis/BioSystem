/*
 * BioSystemTests.h
 *
 *  Created on: Jan 22, 2016
 *      Author: Chris
 */

#ifndef BIOSYSTEMTESTS_H_
#define BIOSYSTEMTESTS_H_


class BioSystemTest
{
public:
	BioSystemTest(){}

	static void NestedIFTest();
	static void SimpleWhileSensorConditional();
	static void NestedIFInWhile();
	static void SimpleProtocol();


	static void SimplePCRLoop();
	static void ProbablisticPCR();
	static void PCRDropletReplacement();
	static void PCR();
	static void GlucoseDetection();
	static void ImageProbSynthesis();
	static void neurotransmitterSensing();
};


#endif /* BIOSYSTEMTESTS_H_ */
