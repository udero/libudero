/**
 * @file kinematics.h
 *
 */

#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include <string>
#include <sstream>
#include <vector>




#include "defines.h"
/**
 * 4x4マトリックス
 */
struct Matrix44 {
public:
  /**
   * マトリックスの値を収納するメンバ
   */
  double v[4][4];
public:

  /**
   * コンストラクタ．全メンバは0で初期化される
   */
  Matrix44() {
    for(int i = 0;i < 4;i++)
      for(int j = 0;j < 4;j++) 
	v[i][j] = 0;
  }

  /**
   * デストラクタ
   */
  ~Matrix44() {}

public:
  
  /**
   * 引数mから値をコピーする
   */
  void copyFrom(const Matrix44& m) {
    for(int i = 0;i < 4;i++)
      for(int j = 0;j < 4;j++) 
	this->v[i][j] = m.v[i][j];
  }

  /**
   * コピーコンストラクタ
   */
  Matrix44(const Matrix44& m) {
    copyFrom(m);
  }

  /**
   * 代入演算
   */
  void operator=(const Matrix44& m) {
    copyFrom(m);
  }

};

/**
 * 乗算．行列の乗算をする
 */
inline Matrix44 operator*(const Matrix44& A, const Matrix44& B) {
  Matrix44 C;

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      for (int k = 0; k < 4; k++) {
	C.v[i][j] += A.v[i][k] * B.v[k][j];
      }
    }
  }
  return C;
}


/**
 * 3要素のベクトル
 */
struct Vector3 {

  /**
   * ベクトルの値を格納するメンバ
   */
  double v[3];

  /**
   * コンストラクタ．全て0で初期化
   */
  Vector3() {
    v[0] = v[1] = v[2] = 0;
  }

  Vector3(const Vector3& v3) {
    v[0] = v3.v[0]; v[1] = v3.v[1]; v[2] = v3.v[2];
  }

  void operator=(const Vector3& v3) {
    v[0] = v3.v[0]; v[1] = v3.v[1]; v[2] = v3.v[2];
  }


};
 


/**
 * 準運動学計算
 */
LIBUDERO_API Matrix44 forward_kinematics(const std::vector<double> pos, int debug = 0);

/**
 * 逆運動学計算
 */
LIBUDERO_API std::vector<double> inverse_kinematics(const Matrix44& mat);

/**
 * 手首位置の準運動学計算
 */
LIBUDERO_API Matrix44 forward_kinematics_wrist(const std::vector<double> pos, int debug = 0);

LIBUDERO_API void forward_kinematics_both(const std::vector<double>& joints, Matrix44& ee, Matrix44& wrist);

/**
 * 手首位置に対する逆運動学計算
 */
std::vector<double> inverse_kinematics_wrist(const Matrix44& mat);

/**
 * 逆運動学計算で得た関節位置を，現在の関節位置を照らし合わせてもっともらしい動作をするための位置に変更するフィルタ関数
 */
std::vector<double> joint_filter2(const std::vector<double>& current, const std::vector<double>& target);

/**
 * 4x4の変換行列をオイラー角(ZYZ)に返還する
 */
inline Vector3 MatrixToEulerZYZ(const Matrix44& hg) {
  Vector3 v;
  double c2 = hg.v[2][2];
  double e = 1.0e-5;
  if (fabs(c2 - 1.0) < e) {
    v.v[0] = 0.0;
    v.v[1] = 0.0;
    double d = hg.v[0][0] + hg.v[1][1];
    v.v[2] = acos(d/2);
    return v;
  }

  v.v[0] = atan2(hg.v[1][2], hg.v[0][2]);  
  v.v[1] = acos(c2);
  v.v[2] = atan2(hg.v[2][1], -hg.v[2][0]);
  return v;
}


/**
 * Matrix44の文字列化
 */
inline std::string str(const Matrix44& m) {
  std::stringstream ss;
  ss << m.v[0][0] << " " << m.v[0][1] << " " << m.v[0][2] << " " << m.v[0][3] << "\n"
     << m.v[1][0] << " " << m.v[1][1] << " " << m.v[1][2] << " " << m.v[1][3] << "\n"
     << m.v[2][0] << " " << m.v[2][1] << " " << m.v[2][2] << " " << m.v[2][3] << "\n"
     << m.v[3][0] << " " << m.v[3][1] << " " << m.v[3][2] << " " << m.v[3][3];
  return ss.str();
}

/**
 * Vector3の文字列化
 */
inline std::string str(const Vector3& v) {
  std::stringstream ss;
  ss << "[[" << v.v[0] << "]\n"
     << " [" << v.v[1] << "]\n"
     << " [" << v.v[2] << "]";
  return ss.str();
}


