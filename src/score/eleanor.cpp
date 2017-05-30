// -*- mode: c++; c-file-style: "ellemtel"; encoding: utf-8; -*-
//
// Copyright (C) 2012 Saulo H. P. de Oliveira
//
// Saulo H. P. de Oliveira <saulo.deoliveira@pmb.ox.ac.uk>
// 2012-08-06
//
// Predicted Contact Scoring Method.
//
// 


#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <iostream>
#include <fstream>
#include <map>
#include "amino.h"
#include "residue.h"
#include "peptide.h"
#include "atom.h"
#include "scorer_combined.h"
#include "eleanor.h"

Eleanor::Eleanor()
{
	m_data_loaded=false;
	const double temp_mempot[20][34] = {
		{2.52005,2.6278,2.55935,2.5911,2.4642,2.5838,2.45685,2.33295,2.23535,2.2615,2.13895,2.18975,2.10015,2.09415,2.06735,2.01235,2.1215,2.1215,2.01235,2.06735,2.09415,2.10015,2.18975,2.13895,2.2615,2.23535,2.33295,2.45685,2.5838,2.4642,2.5911,2.55935,2.6278,2.52005},
		{2.84075,2.90405,3.06315,2.8057,2.6575,2.93865,3.2878,3.1989,3.51125,3.79525,4.33775,4.77555,4.9488,5.38435,5.7338,5.6138,5.5653,5.5653,5.6138,5.7338,5.38435,4.9488,4.77555,4.33775,3.79525,3.51125,3.1989,3.2878,2.93865,2.6575,2.8057,3.06315,2.90405,2.84075             },
		{3.1782,3.11365,2.99165,2.9751,3.13965,3.01275,3.2871,3.6473,4.089,3.98995,3.96895,3.7699,4.13305,4.2522,4.37315,4.27865,4.4588,4.4588,4.27865,4.37315,4.2522,4.13305,3.7699,3.96895,3.98995,4.089,3.6473,3.2871,3.01275,3.13965,2.9751,2.99165,3.11365,3.1782               },
		{2.78485,2.8849,2.77085,2.96605,3.1148,3.3923,3.98025,3.99305,4.43555,4.4271,5.2002,4.78455,4.96285,4.9453,5.00835,5.0553,5.8451,5.8451,5.0553,5.00835,4.9453,4.96285,4.78455,5.2002,4.4271,4.43555,3.99305,3.98025,3.3923,3.1148,2.96605,2.77085,2.8849,2.78485             },
		{4.7224,5.1619,5.0364,5.56805,5.40685,5.3705,5.07355,4.82765,4.96465,4.9638,4.2942,4.591,4.37245,4.49635,4.0677,3.9477,4.31695,4.31695,3.9477,4.0677,4.49635,4.37245,4.591,4.2942,4.9638,4.96465,4.82765,5.07355,5.3705,5.40685,5.56805,5.0364,5.1619,4.7224                 },
		{3.1483,2.87385,3.2914,3.3081,3.3646,3.53025,3.56505,3.6763,3.90735,3.94885,4.3146,4.42675,4.9152,4.99295,4.8968,4.8871,4.7373,4.7373,4.8871,4.8968,4.99295,4.9152,4.42675,4.3146,3.94885,3.90735,3.6763,3.56505,3.53025,3.3646,3.3081,3.2914,2.87385,3.1483                 },
		{2.54045,2.4931,2.8701,3.0078,3.285,3.47465,3.57805,3.8602,4.13665,4.04745,4.44705,4.3801,4.5192,5.38435,4.47225,4.9795,5.08385,5.08385,4.9795,4.47225,5.38435,4.5192,4.3801,4.44705,4.04745,4.13665,3.8602,3.57805,3.47465,3.285,3.0078,2.8701,2.4931,2.54045               },
		{2.56235,2.4725,2.52895,2.38165,2.4212,2.3288,2.4915,2.66225,2.5394,2.582,2.3893,2.42985,2.3515,2.24605,2.2718,2.31005,2.2514,2.2514,2.31005,2.2718,2.24605,2.3515,2.42985,2.3893,2.582,2.5394,2.66225,2.4915,2.3288,2.4212,2.38165,2.52895,2.4725,2.56235                   },
		{3.9525,3.71665,3.68475,3.6836,3.7189,3.6546,3.734,4.07325,4.00195,4.233,4.2072,4.3945,4.3903,4.38105,4.617,4.7768,5.21875,5.21875,4.7768,4.617,4.38105,4.3903,4.3945,4.2072,4.233,4.00195,4.07325,3.734,3.6546,3.7189,3.6836,3.68475,3.71665,3.9525                         },
		{2.9406,3.1338,3.3103,3.1099,3.15695,2.80435,2.6678,2.5503,2.501,2.3334,2.2023,2.30285,2.1846,2.12795,2.10245,2.13,2.129,2.129,2.13,2.10245,2.12795,2.1846,2.30285,2.2023,2.3334,2.501,2.5503,2.6678,2.80435,3.15695,3.1099,3.3103,3.1338,2.9406                             },
		{2.568,2.6949,2.49605,2.52565,2.48235,2.32635,2.07275,2.0961,1.90765,1.86095,1.87575,1.81015,1.78135,1.7058,1.74745,1.75885,1.72515,1.72515,1.75885,1.74745,1.7058,1.78135,1.81015,1.87575,1.86095,1.90765,2.0961,2.07275,2.32635,2.48235,2.52565,2.49605,2.6949,2.568       },
		{2.841,3.10695,2.8844,3.02425,2.9932,3.24915,3.3062,3.56155,3.90455,4.09025,4.9307,5.26595,6.19125,5.32545,5.01625,6.1631,5.73355,5.73355,6.1631,5.01625,5.32545,6.19125,5.26595,4.9307,4.09025,3.90455,3.56155,3.3062,3.24915,2.9932,3.02425,2.8844,3.10695,2.841           },
		{3.84505,3.8891,3.69685,4.07025,3.50355,3.65675,3.562,3.39395,3.27815,3.32615,3.3333,3.26705,3.3149,3.3341,3.40175,3.3398,3.166,3.166,3.3398,3.40175,3.3341,3.3149,3.26705,3.3333,3.32615,3.27815,3.39395,3.562,3.65675,3.50355,4.07025,3.69685,3.8891,3.84505               },
		{3.3715,3.44085,3.2659,3.2435,3.0166,2.7211,2.66215,2.3924,2.4648,2.45465,2.42325,2.36095,2.3534,2.46985,2.60015,2.61145,2.4065,2.4065,2.61145,2.60015,2.46985,2.3534,2.36095,2.42325,2.45465,2.4648,2.3924,2.66215,2.7211,3.0166,3.2435,3.2659,3.44085,3.3715               },
		{2.8806,2.66925,2.74535,2.7652,3.0437,3.04005,3.12165,3.11435,3.27855,3.42515,3.59485,3.7013,3.54625,3.63235,3.7872,3.74895,3.66195,3.66195,3.74895,3.7872,3.63235,3.54625,3.7013,3.59485,3.42515,3.27855,3.11435,3.12165,3.04005,3.0437,2.7652,2.74535,2.66925,2.8806       },
		{2.82165,2.8116,2.71975,2.46025,2.72915,2.8093,2.90585,2.90475,3.05305,3.0549,3.01175,2.9875,2.9451,2.9508,2.8836,2.9456,3.0527,3.0527,2.9456,2.8836,2.9508,2.9451,2.9875,3.01175,3.0549,3.05305,2.90475,2.90585,2.8093,2.72915,2.46025,2.71975,2.8116,2.82165               },
		{2.8751,2.98995,2.7865,2.9739,2.8529,2.9445,2.88255,3.09235,2.96095,2.88055,3.01895,2.98455,2.97075,3.00755,2.9856,2.89255,2.926,2.926,2.89255,2.9856,3.00755,2.97075,2.98455,3.01895,2.88055,2.96095,3.09235,2.88255,2.9445,2.8529,2.9739,2.7865,2.98995,2.8751             },
		{4.73155,4.70375,4.24985,4.13695,3.5969,3.39555,3.3167,3.1658,3.244,3.31935,3.3067,3.48425,3.7784,3.88025,3.92385,3.9388,4.0615,4.0615,3.9388,3.92385,3.88025,3.7784,3.48425,3.3067,3.31935,3.244,3.1658,3.3167,3.39555,3.5969,4.13695,4.24985,4.70375,4.73155               },
		{3.56755,3.23415,3.16165,3.39295,3.0857,3.22185,3.19205,2.9531,3.05305,2.99895,3.51165,3.3121,3.4899,3.6943,3.8297,3.80385,3.99755,3.99755,3.80385,3.8297,3.6943,3.4899,3.3121,3.51165,2.99895,3.05305,2.9531,3.19205,3.22185,3.0857,3.39295,3.16165,3.23415,3.56755         },
		{2.64975,2.97085,3.0263,2.87695,2.9102,2.79455,2.6516,2.6345,2.4052,2.3888,2.22665,2.18765,2.25655,2.2548,2.1427,2.1209,2.0847,2.0847,2.1209,2.1427,2.2548,2.25655,2.18765,2.22665,2.3888,2.4052,2.6345,2.6516,2.79455,2.9102,2.87695,3.0263,2.97085,2.64975                 }};
	//memcpy(m_mempot, temp_mempot, sizeof m_mempot);
	for (int i=0; i<20; i++)
	{
		for (int j=0; j<34; j++)
		{
			m_mempot[i][j] = temp_mempot[i][j];
		}
	}

	const std::string temp_resis_list[20] = {"ALA","ARG","ASN","ASP","CYS","GLN","GLU","GLY","HIS","ILE","LEU","LYS","MET","PHE","PRO","SER","THR","TRP","TYR","VAL"};
	//memcpy(zpot_residues_list, temp_resis_list, sizeof zpot_residues_list);
	for (int i=0; i<20; i++)
	{
		zpot_residues_list[i] = temp_resis_list[i];
	}
}

Eleanor::~Eleanor()
{
}

void Eleanor::set_short_data_file(const std::string &filename)
{
	m_filename = filename;
}

void Eleanor::set_long_data_file(const std::string &filename)
{
	m_filename  = filename;
}

void Eleanor::load_data(const Peptide& p)
{
	if (m_data_loaded)
	{
		return;
	}
	
	// for every position in chain get index corresponding to amino acid
	for (int i=0; i<p.full_length(); i++)
	{
		m_aaindeces[i] = -1;
		// loop over all the amino acids
		for (int aaindex=0; aaindex<20; aaindex++)
		{
			if (std::strcmp((zpot_residues_list[aaindex]).c_str(), p.res(i).amino().abbr()) == 0)
			{
				//std::cout << "matching: ---" << p.res(i).amino().abbr() << "-" << zpot_residues_list[aaindex] << "---\n";
				m_aaindeces[i] = aaindex;
				//std::cout << "resi " << i << ": " << aaindex << ", " << p.res(i).amino().abbr() << "\n";
			}
			else
			{
				//std::cout << "no match: ---" << p.res(i).amino().abbr() << "-" << zpot_residues_list[aaindex] << "---\n";
			}

		}

		// if match not found in zpot
		if (m_aaindeces[i] == -1)
		{
			std::cout << "no matching residue name in z-potential: ---" << p.res(i).amino().abbr() << "---\n";
			exit(1);
		}
	}


	m_previous_len = 0;
	m_data_loaded = true;
}

double Eleanor::score(const Peptide& p, bool verbose)
{
	load_data(p);

	int len = p.length();
	int layer;
	int bin;
	float zcoord;

	double total=0.0;

	if(len != m_previous_len)
	{
		m_previous_len=len;
	}

	for (int k=0; k<len; k++)
	{
		if (m_aaindeces[k] == 7)
		{
			zcoord = p.atom_pos(k, Atom_CA).z;
		}
		else
		{
			zcoord = p.atom_pos(k, Atom_CB).z;
		}
		//std::cout << "zcoord: " << zcoord << "\n";
		layer = std::min((int) std::floor(fabs(zcoord)/1.5),17);
		//std::cout << "layer: " << layer << "\n";
		bin = 17 + layer;
		//std::cout << "bin: " << bin << "\n";
		//std::cout << "total before: " << total << "\n";
		total += m_mempot[m_aaindeces[k]][bin];
		//std::cout << "total after: " << total << "\n";
	}

	
#ifndef RAW_SCORE

	// Alter the score so that it has about the same distribution
	// for all lengths
	total /= len;
	// normalise so that all score types have approximately the same range
	total = total * 1000;
	
#endif // RAW_SCORE
//	std::cout << "Total = " << total << " !!\n";	
	return total;
}

