#include "udero/kinematics.h"
#include <vector>
#include <iostream>

static const double L1 = 135;
static const double L2 = 270;
static const double L3 = 40;
static const double L4 = 103.7+136.3;
static const double L5 = 130;


#include <Eigen/Core>
//#include <Eigen/SVD>
//#include <Eigen/LU>

inline double normalize_angle(double angle) {
  while(angle > M_PI) angle -= 2 * M_PI;
  while(angle < -M_PI) angle += 2 * M_PI;
  return angle;
}

typedef Eigen::Matrix<double, 4, 4> MATRIX44;
typedef Eigen::Matrix<double, 6, 6> MATRIX66;
typedef Eigen::Vector4d VECTOR4;
typedef Eigen::Matrix<double, 6, 1> VECTOR6;


static MATRIX44 RotTransX(const double t, const double x, const double y, const double z) {
  MATRIX44 result = MATRIX44::Identity(4,4);
  double c = cos(t); double s = sin(t);
  //result(0, 0) = 1; result(0, 1) = 0; result(0, 2) = 0;
  /* result(1, 0) = 0;*/ result(1, 1) = c; result(1, 2) =-s;
  /* result(2, 0) = 0;*/ result(2, 1) = s; result(2, 2) = c;
  result(0, 3) = x; result(1, 3) = y; result(2, 3) = z;
  return result;
}


static MATRIX44 RotTransY(const double t, const double x, const double y, const double z) {
  MATRIX44 result = MATRIX44::Identity(4,4);
  double c = cos(t); double s = sin(t);
  result(0, 0) = c; /*result(0, 1) = 0;*/ result(0, 2) = s;
  //result(1, 0) = 0; result(1, 1) = 1; result(1, 2) = 0; 
  result(2, 0) =-s; /*result(2, 1) = 0;*/ result(2, 2) = c;
  result(0, 3) = x; result(1, 3) = y; result(2, 3) = z;
  return result;
}

static MATRIX44 RotTransZ(const double t, const double x, const double y, const double z) {
  MATRIX44 result = MATRIX44::Identity(4,4);
  double c = cos(t); double s = sin(t);
  result(0, 0) = c; result(0, 1) = -s; /*result(0, 2) = 0;*/
  result(1, 0) = s; result(1, 1) = c; /*result(1, 2) = 0;*/
  //result(2, 0) = 0; result(2, 1) = 0; result(2, 2) = 1; 
  result(0, 3) = x; result(1, 3) = y; result(2, 3) = z;

  return result;
}


Matrix44 mat_to_mat(const MATRIX44& mat) {
  Matrix44 m;
  for(int i = 0;i < 4;i++) {
    for(int j = 0;j < 4;j++) {
      m.v[i][j] = mat(i, j);
    }
  }
  
  return m;
}


MATRIX44 mat_to_mat(const Matrix44& mat) {
  MATRIX44 m;
  for(int i = 0;i < 4;i++) {
    for(int j = 0;j < 4;j++) {
      m(i, j) = mat.v[i][j];
    }
  }
  return m;
}


std::vector<double> joint_filter2(const std::vector<double>& current, const std::vector<double>& target) {
	std::vector<double> current_norm;
	/// for (auto i : current) {
	
	//	double j = i;
	for(std::vector<double>::const_iterator it=current.begin();it != current.end();++it) {
	  double j = *it;
		while (j < -M_PI) j += M_PI * 2;
		while (j > M_PI) j -= M_PI * 2;
		current_norm.push_back(j);
	}

	std::vector<double> target_alt;
	double j;
	target_alt.push_back(target[0]);
	target_alt.push_back(target[1]);
	target_alt.push_back(target[2]);
	j = target[3] + M_PI;
	while (j < -M_PI) j += M_PI * 2;
	while (j > M_PI) j -= M_PI * 2;
	target_alt.push_back(j);
	//target_alt.push_back(target[3]);
	target_alt.push_back(-target[4]);
	j = target[5] + M_PI;
	while (j < -M_PI) j += M_PI * 2;
	while (j > M_PI) j -= M_PI * 2;
	target_alt.push_back(j);
	//target_alt.push_back(-target[5]);

	std::vector<double> ret;
	// J[3]‚ª‹ß‚¢‚Ù‚¤‚ð‘I‚Ô
	if (fabs(current_norm[3] - target[3]) < fabs(current_norm[3] - target_alt[3])) {
		ret = target;
	}
	else {
		ret = target_alt;
	}
	
	// ÅŒã‚É‰ñ“]—Ê‚ð‡‚í‚¹‚é

	size_t s = ret.size();
	for (int i = 0; i < s; i++) {
		ret[i] = ret[i] + current[i] - current_norm[i];
	}

	return ret;
}

LIBUDERO_API Matrix44 forward_kinematics(const std::vector<double> joints, int debug) {
  MATRIX44 jointMat[9];
  jointMat[0] = RotTransZ(joints[0], 0, 0, 0);
  jointMat[1] = RotTransY(joints[1], 0, 0, L1);
  jointMat[2] = RotTransY(0, 0, 0, L2);;
  jointMat[3] = RotTransY(joints[2], L3, 0, 0);
  jointMat[4] = RotTransY(0, -L3, 0, 0);
  jointMat[5] = RotTransZ(joints[3], 0, 0, L4);
  jointMat[6] = RotTransY(joints[4], 0, 0, 0);
  jointMat[7] = RotTransZ(joints[5], 0, 0, 0);
  jointMat[8] = RotTransZ(0, 0, 0, L5);

  MATRIX44 jointMatAbs[9];
  jointMatAbs[0] = jointMat[0];
  for(int i = 0;i < 8;i++) {
    jointMatAbs[i+1] = jointMatAbs[i] * jointMat[i+1];
    if (debug) {
      std::cout << "J" << i+1 << std::endl << jointMatAbs[i+1] << std::endl;
    }
  }
  return mat_to_mat(jointMatAbs[8]);
}

LIBUDERO_API Matrix44 forward_kinematics_wrist(const std::vector<double> joints, int debug) {
  MATRIX44 jointMat[9];
  jointMat[0] = RotTransZ(joints[0], 0, 0, 0);
  jointMat[1] = RotTransY(joints[1], 0, 0, L1);
  jointMat[2] = RotTransY(0, 0, 0, L2);;
  jointMat[3] = RotTransY(joints[2], L3, 0, 0);
  jointMat[4] = RotTransY(0, -L3, 0, 0);
  jointMat[5] = RotTransZ(joints[3], 0, 0, L4);
  jointMat[6] = RotTransY(joints[4], 0, 0, 0);
  jointMat[7] = RotTransZ(joints[5], 0, 0, 0);
  jointMat[8] = RotTransZ(0, 0, 0, 0);
  
  MATRIX44 jointMatAbs[9];
  jointMatAbs[0] = jointMat[0];
  for (int i = 0; i < 8; i++) {
    jointMatAbs[i + 1] = jointMatAbs[i] * jointMat[i + 1];
    if (debug) {
      std::cout << "J" << i + 1 << std::endl << jointMatAbs[i + 1] << std::endl;
    }
  }
  return mat_to_mat(jointMatAbs[8]);
}

LIBUDERO_API void forward_kinematics_both(const std::vector<double>& joints, Matrix44& ee, Matrix44& wrist) {
  MATRIX44 jointMat[9];
  jointMat[0] = RotTransZ(joints[0], 0, 0, 0);
  jointMat[1] = RotTransY(joints[1], 0, 0, L1);
  jointMat[2] = RotTransY(0, 0, 0, L2);;
  jointMat[3] = RotTransY(joints[2], L3, 0, 0);
  jointMat[4] = RotTransY(0, -L3, 0, 0);
  jointMat[5] = RotTransZ(joints[3], 0, 0, L4);
  jointMat[6] = RotTransY(joints[4], 0, 0, 0);
  jointMat[7] = RotTransZ(joints[5], 0, 0, 0);
  jointMat[8] = RotTransZ(0, 0, 0, L5);

  MATRIX44 jointMatAbs[9];
  jointMatAbs[0] = jointMat[0];
  for(int i = 0;i < 8;i++) {
    jointMatAbs[i+1] = jointMatAbs[i] * jointMat[i+1];
  }
  ee = mat_to_mat(jointMatAbs[8]);
  wrist = mat_to_mat(jointMatAbs[7]);
}



std::vector<double> inverse_kinematics(const Matrix44& mat) {
	MATRIX44 m = mat_to_mat(mat);
	VECTOR4  v(0, 0, -L5, 1);
	VECTOR4  p5 = m * v;
	VECTOR4  p2(0, 0, L1, 0);
	double d0 = sqrt(L2*L2 + L3*L3);
	double d1 = sqrt(L3*L3 + L4*L4);
	VECTOR4  d25 = p5 - p2;
	double   L = d25.norm();
	double   e = 1.0e-0;

	double th3_;
	if (fabs(L - (d0 + d1)) < 1.0e-10) {
		th3_ = 0;
	}
	else {
		th3_ = acos((d0*d0 + d1*d1 - L*L) / 2 / d0 / d1);// - th_offst;
	}

	double th3 = 2 * M_PI - (asin(L2 / d0) + asin(L4 / d1) + th3_);
	double th1 = atan2(p5(1), p5(0));

	double th2_;
  if (fabs(L - (d1+d0)) < 1.0e-10) {
    th2_ = atan2(sqrt(p5(0)*p5(0)+p5(1)*(p5(1))), p5(2)-L1);//- (M_PI/2/* - th_offst*/);
  } else {
    th2_ = atan2(sqrt(p5(0)*p5(0)+p5(1)*(p5(1))), p5(2)-L1)
      - acos((d0*d0 + L*L - d1*d1) / 2 / d0 / L);// - (M_PI/2/* - th_offst*/);
  }
  
  double th2 = th2_ - asin(L3 / d0);
  
  MATRIX44 mm = RotTransY(-th3, 0, 0, 0) * RotTransY(-th2, 0, 0, 0) * RotTransZ(-th1, 0, 0, 0) * m;
  Vector3 vv = MatrixToEulerZYZ(mat_to_mat(mm));
  std::vector<double> j;
  j.push_back(th1);
  j.push_back(th2);
  j.push_back(th3);
  j.push_back(vv.v[0]);
  j.push_back(vv.v[1]);
  j.push_back(vv.v[2]);
  return j;
}

std::vector<double> inverse_kinematics_wrist(const Matrix44& mat) {
	MATRIX44 m = mat_to_mat(mat);
	VECTOR4  v(0, 0, -0, 1);
	VECTOR4  p5 = m * v;
	VECTOR4  p2(0, 0, L1, 0);
	double d0 = sqrt(L2*L2 + L3*L3);
	double d1 = sqrt(L3*L3 + L4*L4);
	VECTOR4  d25 = p5 - p2;
	double   L = d25.norm();
	double   e = 1.0e-0;

	double th3_;
	if (fabs(L - (d0 + d1)) < 1.0e-10) {
		th3_ = 0;
	}
	else {
		th3_ = acos((d0*d0 + d1*d1 - L*L) / 2 / d0 / d1);// - th_offst;
	}

	double th3 = 2 * M_PI - (asin(L2 / d0) + asin(L4 / d1) + th3_);
	double th1 = atan2(p5(1), p5(0));

	double th2_;
	if (fabs(L - (d1 + d0)) < 1.0e-10) {
		th2_ = atan2(sqrt(p5(0)*p5(0) + p5(1)*(p5(1))), p5(2) - L1);//- (M_PI/2/* - th_offst*/);
	}
	else {
		th2_ = atan2(sqrt(p5(0)*p5(0) + p5(1)*(p5(1))), p5(2) - L1)
			- acos((d0*d0 + L*L - d1*d1) / 2 / d0 / L);// - (M_PI/2/* - th_offst*/);
	}

	double th2 = th2_ - asin(L3 / d0);

	MATRIX44 mm = RotTransY(-th3, 0, 0, 0) * RotTransY(-th2, 0, 0, 0) * RotTransZ(-th1, 0, 0, 0) * m;
	Vector3 vv = MatrixToEulerZYZ(mat_to_mat(mm));
	std::vector<double> j;
	j.push_back(th1);
	j.push_back(th2);
	j.push_back(th3);
	j.push_back(vv.v[0]);
	j.push_back(vv.v[1]);
	j.push_back(vv.v[2]);
	return j;
}
