/* ===============================================================================================
Anton's 3D Maths Library (C99 version)
URL:     https://github.com/capnramses/apg
Licence: see bottom of file.
Author:  Anton Gerdelan <antonofnote at gmail> @capnramses
==================================================================================================
History:
 9 Apr 2020 - v2.10 Frustum extraction functions.
11 Apr 2016 - v2.9  Compacted.
12 Apr 2016 - v2.8  Switched to .x .y .z notation for vectors and quaternions.
17 Jul 2019 - v2.7  Updated to code from voxel game project.
20 Nov 2019 - v2.6  Added a cpp clause.
 5 May 2015 - v2.5  Branched from C++ original.
==================================================================================================
TODO:
project and reject vectors
-Matrix
TODO arbitrary axis rot
-Virtual Camera
TODO orthographic
-Quaternions
TODO conjugates
-Geometry
TODO distance point to line
TODO distance line to line
TODO distance point to plane
~plane reflection matrix
TODO line-plane intersect
TODO line-sphere intersect
TODO line-OBB intersect
TODO line-AABB intersect
-bit storage in rgb ~
int r = (num & 0xFF0000) >> 16
int g = (num & 0x00FF00) >> 8
int b = (num & 0x0000FF) >> 0 // or lose the >>0
=============================================================================================== */
#pragma once

#include <assert.h>
#include <float.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef M_PI // C99 removed M_PI
#define M_PI 3.14159265358979323846
#define M_PI_2 M_PI / 2.0
#endif

#define HALF_PI M_PI_2
#define ONE_DEG_IN_RAD ( 2.0f * M_PI ) / 360.0f // 0.017444444
#define ONE_RAD_IN_DEG 360.0 / ( 2.0f * M_PI )  // 57.2957795

#define MIN( a, b ) ( ( a ) < ( b ) ? ( a ) : ( b ) )
#define MAX( a, b ) ( ( a ) > ( b ) ? ( a ) : ( b ) )
#define CLAMP( x, lo, hi ) ( MIN( hi, MAX( lo, x ) ) )

typedef struct vec2 {
  float x, y;
} vec2;

typedef struct vec3 {
  float x, y, z;
} vec3;

typedef struct vec4 {
  float x, y, z, w;
} vec4;

typedef struct ivec3 {
  int x, y, z;
} ivec3;

typedef struct mat4 {
  float m[16];
} mat4;

typedef struct versor {
  float w, x, y, z;
} versor;

static inline void print_vec2( vec2 v ) { printf( "[%.2f, %.2f]\n", v.x, v.y ); }
static inline void print_vec3( vec3 v ) { printf( "[%.2f, %.2f, %.2f]\n", v.x, v.y, v.z ); }
static inline void print_vec4( vec4 v ) { printf( "[%.2f, %.2f, %.2f, %.2f]\n", v.x, v.y, v.z, v.w ); }
static inline void print_mat4( mat4 m ) {
  printf( "\n" );
  printf( "[%.2f][%.2f][%.2f][%.2f]\n", m.m[0], m.m[4], m.m[8], m.m[12] );
  printf( "[%.2f][%.2f][%.2f][%.2f]\n", m.m[1], m.m[5], m.m[9], m.m[13] );
  printf( "[%.2f][%.2f][%.2f][%.2f]\n", m.m[2], m.m[6], m.m[10], m.m[14] );
  printf( "[%.2f][%.2f][%.2f][%.2f]\n", m.m[3], m.m[7], m.m[11], m.m[15] );
}
static inline void print_quat( versor q ) { printf( "[%.2f ,%.2f, %.2f, %.2f]\n", q.w, q.x, q.y, q.z ); }

static inline vec3 v3_v4( vec4 v ) { return ( vec3 ){ .x = v.x, .y = v.y, .z = v.z }; }
static inline vec4 v4_v3f( vec3 v, float f ) { return ( vec4 ){ .x = v.x, .y = v.y, .z = v.z, .w = f }; }

static inline vec3 add_vec3_f( vec3 a, float b ) { return ( vec3 ){ .x = a.x + b, .y = a.y + b, .z = a.z + b }; }
static inline vec3 add_vec3_vec3( vec3 a, vec3 b ) { return ( vec3 ){ .x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z }; }

static inline vec3 sub_vec3_f( vec3 a, float b ) { return ( vec3 ){ .x = a.x - b, .y = a.y - b, .z = a.z - b }; }
static inline vec2 sub_vec2_vec2( vec2 a, vec2 b ) { return ( vec2 ){ .x = a.x - b.x, .y = a.y - b.y }; }
static inline vec3 sub_vec3_vec3( vec3 a, vec3 b ) { return ( vec3 ){ .x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z }; }

static inline vec3 mult_vec3_f( vec3 a, float b ) { return ( vec3 ){ .x = a.x * b, .y = a.y * b, .z = a.z * b }; }
static inline vec3 mult_vec3_vec3( vec3 a, vec3 b ) { return ( vec3 ){ .x = a.x * b.x, .y = a.y * b.y, .z = a.z * b.z }; }

static inline vec3 div_vec3_f( vec3 a, float b ) { return ( vec3 ){ .x = a.x / b, .y = a.y / b, .z = a.z / b }; }
static inline vec3 div_vec3_vec3( vec3 a, vec3 b ) { return ( vec3 ){ .x = a.x / b.x, .y = a.y / b.y, .z = a.z / b.z }; }
static inline vec4 div_vec4_f( vec4 v, float f ) { return ( vec4 ){ .x = v.x / f, .y = v.y / f, .z = v.z / f, .w = v.w / f }; }

// magnitude or length of a vec2
static inline float length_vec2( vec2 v ) { return sqrt( v.x * v.x + v.y * v.y ); }
// squared length
static inline float length2_vec2( vec2 v ) { return v.x * v.x + v.y * v.y; }
// magnitude or length of a vec3
static inline float length_vec3( vec3 v ) { return sqrtf( v.x * v.x + v.y * v.y + v.z * v.z ); }
// squared length
static inline float length2_vec3( vec3 v ) { return v.x * v.x + v.y * v.y + v.z * v.z; }

static inline vec3 normalise_vec3( vec3 v ) {
  vec3 vb;
  float l = length_vec3( v );
  if ( 0.0f == l ) { return ( vec3 ){ .x = 0.0f, .y = 0.0f, .z = 0.0f }; }
  vb.x = v.x / l;
  vb.y = v.y / l;
  vb.z = v.z / l;
  return vb;
}

static inline float dot_vec3( vec3 a, vec3 b ) { return a.x * b.x + a.y * b.y + a.z * b.z; }

static inline vec3 cross_vec3( vec3 a, vec3 b ) { return ( vec3 ){ .x = a.y * b.z - a.z * b.y, .y = a.z * b.x - a.x * b.z, .z = a.x * b.y - a.y * b.x }; }

// converts an un-normalised direction vector's X,Z components into a heading in degrees
static inline float vec3_to_heading( vec3 d ) { return atan2f( -d.x, -d.z ) * ONE_RAD_IN_DEG; }
// very informal function to convert a heading (e.g. y-axis orientation) into a 3d vector with components in x and z axes
static inline vec3 heading_to_vec3( float degrees ) {
  float rad = degrees * ONE_DEG_IN_RAD;
  return ( vec3 ){ .x = -sinf( rad ), .y = 0.0f, .z = -cosf( rad ) };
}

static inline mat4 identity_mat4() {
  mat4 r  = { { 0 } };
  r.m[0]  = 1.0f;
  r.m[5]  = 1.0f;
  r.m[10] = 1.0f;
  r.m[15] = 1.0f;
  return r;
}

static inline mat4 mult_mat4_mat4( mat4 a, mat4 b ) {
  mat4 r      = { { 0 } };
  int r_index = 0;
  for ( int col = 0; col < 4; col++ ) {
    for ( int row = 0; row < 4; row++ ) {
      float sum = 0.0f;
      for ( int i = 0; i < 4; i++ ) { sum += b.m[i + col * 4] * a.m[row + i * 4]; }
      r.m[r_index] = sum;
      r_index++;
    }
  }
  return r;
}

static inline vec4 mult_mat4_vec4( mat4 m, vec4 v ) {
  float x = m.m[0] * v.x + m.m[4] * v.y + m.m[8] * v.z + m.m[12] * v.w;
  float y = m.m[1] * v.x + m.m[5] * v.y + m.m[9] * v.z + m.m[13] * v.w;
  float z = m.m[2] * v.x + m.m[6] * v.y + m.m[10] * v.z + m.m[14] * v.w;
  float w = m.m[3] * v.x + m.m[7] * v.y + m.m[11] * v.z + m.m[15] * v.w;
  return ( vec4 ){ .x = x, .y = y, .z = z, .w = w };
}

static inline float det_mat4( mat4 mm ) {
  return mm.m[12] * mm.m[9] * mm.m[6] * mm.m[3] - mm.m[8] * mm.m[13] * mm.m[6] * mm.m[3] - mm.m[12] * mm.m[5] * mm.m[10] * mm.m[3] +
         mm.m[4] * mm.m[13] * mm.m[10] * mm.m[3] + mm.m[8] * mm.m[5] * mm.m[14] * mm.m[3] - mm.m[4] * mm.m[9] * mm.m[14] * mm.m[3] -
         mm.m[12] * mm.m[9] * mm.m[2] * mm.m[7] + mm.m[8] * mm.m[13] * mm.m[2] * mm.m[7] + mm.m[12] * mm.m[1] * mm.m[10] * mm.m[7] -
         mm.m[0] * mm.m[13] * mm.m[10] * mm.m[7] - mm.m[8] * mm.m[1] * mm.m[14] * mm.m[7] + mm.m[0] * mm.m[9] * mm.m[14] * mm.m[7] +
         mm.m[12] * mm.m[5] * mm.m[2] * mm.m[11] - mm.m[4] * mm.m[13] * mm.m[2] * mm.m[11] - mm.m[12] * mm.m[1] * mm.m[6] * mm.m[11] +
         mm.m[0] * mm.m[13] * mm.m[6] * mm.m[11] + mm.m[4] * mm.m[1] * mm.m[14] * mm.m[11] - mm.m[0] * mm.m[5] * mm.m[14] * mm.m[11] -
         mm.m[8] * mm.m[5] * mm.m[2] * mm.m[15] + mm.m[4] * mm.m[9] * mm.m[2] * mm.m[15] + mm.m[8] * mm.m[1] * mm.m[6] * mm.m[15] -
         mm.m[0] * mm.m[9] * mm.m[6] * mm.m[15] - mm.m[4] * mm.m[1] * mm.m[10] * mm.m[15] + mm.m[0] * mm.m[5] * mm.m[10] * mm.m[15];
}

// TODO(Anton) look up fast inverse video tutorial
static inline mat4 inverse_mat4( mat4 mm ) {
  float det = det_mat4( mm );
  if ( 0.0f == det ) { return mm; }
  float inv_det = 1.0f / det;
  mat4 r;
  r.m[0]  = inv_det * ( mm.m[9] * mm.m[14] * mm.m[7] - mm.m[13] * mm.m[10] * mm.m[7] + mm.m[13] * mm.m[6] * mm.m[11] - mm.m[5] * mm.m[14] * mm.m[11] -
                       mm.m[9] * mm.m[6] * mm.m[15] + mm.m[5] * mm.m[10] * mm.m[15] );
  r.m[1]  = inv_det * ( mm.m[13] * mm.m[10] * mm.m[3] - mm.m[9] * mm.m[14] * mm.m[3] - mm.m[13] * mm.m[2] * mm.m[11] + mm.m[1] * mm.m[14] * mm.m[11] +
                       mm.m[9] * mm.m[2] * mm.m[15] - mm.m[1] * mm.m[10] * mm.m[15] );
  r.m[2]  = inv_det * ( mm.m[5] * mm.m[14] * mm.m[3] - mm.m[13] * mm.m[6] * mm.m[3] + mm.m[13] * mm.m[2] * mm.m[7] - mm.m[1] * mm.m[14] * mm.m[7] -
                       mm.m[5] * mm.m[2] * mm.m[15] + mm.m[1] * mm.m[6] * mm.m[15] );
  r.m[3]  = inv_det * ( mm.m[9] * mm.m[6] * mm.m[3] - mm.m[5] * mm.m[10] * mm.m[3] - mm.m[9] * mm.m[2] * mm.m[7] + mm.m[1] * mm.m[10] * mm.m[7] +
                       mm.m[5] * mm.m[2] * mm.m[11] - mm.m[1] * mm.m[6] * mm.m[11] );
  r.m[4]  = inv_det * ( mm.m[12] * mm.m[10] * mm.m[7] - mm.m[8] * mm.m[14] * mm.m[7] - mm.m[12] * mm.m[6] * mm.m[11] + mm.m[4] * mm.m[14] * mm.m[11] +
                       mm.m[8] * mm.m[6] * mm.m[15] - mm.m[4] * mm.m[10] * mm.m[15] );
  r.m[5]  = inv_det * ( mm.m[8] * mm.m[14] * mm.m[3] - mm.m[12] * mm.m[10] * mm.m[3] + mm.m[12] * mm.m[2] * mm.m[11] - mm.m[0] * mm.m[14] * mm.m[11] -
                       mm.m[8] * mm.m[2] * mm.m[15] + mm.m[0] * mm.m[10] * mm.m[15] );
  r.m[6]  = inv_det * ( mm.m[12] * mm.m[6] * mm.m[3] - mm.m[4] * mm.m[14] * mm.m[3] - mm.m[12] * mm.m[2] * mm.m[7] + mm.m[0] * mm.m[14] * mm.m[7] +
                       mm.m[4] * mm.m[2] * mm.m[15] - mm.m[0] * mm.m[6] * mm.m[15] );
  r.m[7]  = inv_det * ( mm.m[4] * mm.m[10] * mm.m[3] - mm.m[8] * mm.m[6] * mm.m[3] + mm.m[8] * mm.m[2] * mm.m[7] - mm.m[0] * mm.m[10] * mm.m[7] -
                       mm.m[4] * mm.m[2] * mm.m[11] + mm.m[0] * mm.m[6] * mm.m[11] );
  r.m[8]  = inv_det * ( mm.m[8] * mm.m[13] * mm.m[7] - mm.m[12] * mm.m[9] * mm.m[7] + mm.m[12] * mm.m[5] * mm.m[11] - mm.m[4] * mm.m[13] * mm.m[11] -
                       mm.m[8] * mm.m[5] * mm.m[15] + mm.m[4] * mm.m[9] * mm.m[15] );
  r.m[9]  = inv_det * ( mm.m[12] * mm.m[9] * mm.m[3] - mm.m[8] * mm.m[13] * mm.m[3] - mm.m[12] * mm.m[1] * mm.m[11] + mm.m[0] * mm.m[13] * mm.m[11] +
                       mm.m[8] * mm.m[1] * mm.m[15] - mm.m[0] * mm.m[9] * mm.m[15] );
  r.m[10] = inv_det * ( mm.m[4] * mm.m[13] * mm.m[3] - mm.m[12] * mm.m[5] * mm.m[3] + mm.m[12] * mm.m[1] * mm.m[7] - mm.m[0] * mm.m[13] * mm.m[7] -
                        mm.m[4] * mm.m[1] * mm.m[15] + mm.m[0] * mm.m[5] * mm.m[15] );
  r.m[11] = inv_det * ( mm.m[8] * mm.m[5] * mm.m[3] - mm.m[4] * mm.m[9] * mm.m[3] - mm.m[8] * mm.m[1] * mm.m[7] + mm.m[0] * mm.m[9] * mm.m[7] +
                        mm.m[4] * mm.m[1] * mm.m[11] - mm.m[0] * mm.m[5] * mm.m[11] );
  r.m[12] = inv_det * ( mm.m[12] * mm.m[9] * mm.m[6] - mm.m[8] * mm.m[13] * mm.m[6] - mm.m[12] * mm.m[5] * mm.m[10] + mm.m[4] * mm.m[13] * mm.m[10] +
                        mm.m[8] * mm.m[5] * mm.m[14] - mm.m[4] * mm.m[9] * mm.m[14] );
  r.m[13] = inv_det * ( mm.m[8] * mm.m[13] * mm.m[2] - mm.m[12] * mm.m[9] * mm.m[2] + mm.m[12] * mm.m[1] * mm.m[10] - mm.m[0] * mm.m[13] * mm.m[10] -
                        mm.m[8] * mm.m[1] * mm.m[14] + mm.m[0] * mm.m[9] * mm.m[14] );
  r.m[14] = inv_det * ( mm.m[12] * mm.m[5] * mm.m[2] - mm.m[4] * mm.m[13] * mm.m[2] - mm.m[12] * mm.m[1] * mm.m[6] + mm.m[0] * mm.m[13] * mm.m[6] +
                        mm.m[4] * mm.m[1] * mm.m[14] - mm.m[0] * mm.m[5] * mm.m[14] );
  r.m[15] = inv_det * ( mm.m[4] * mm.m[9] * mm.m[2] - mm.m[8] * mm.m[5] * mm.m[2] + mm.m[8] * mm.m[1] * mm.m[6] - mm.m[0] * mm.m[9] * mm.m[6] -
                        mm.m[4] * mm.m[1] * mm.m[10] + mm.m[0] * mm.m[5] * mm.m[10] );
  return r;
}

static inline mat4 transpose_mat4( mat4 mm ) {
  mat4 r;
  r.m[0]  = mm.m[0];
  r.m[4]  = mm.m[1];
  r.m[8]  = mm.m[2];
  r.m[12] = mm.m[3];
  r.m[1]  = mm.m[4];
  r.m[5]  = mm.m[5];
  r.m[9]  = mm.m[6];
  r.m[13] = mm.m[7];
  r.m[2]  = mm.m[8];
  r.m[6]  = mm.m[9];
  r.m[10] = mm.m[10];
  r.m[14] = mm.m[11];
  r.m[3]  = mm.m[12];
  r.m[7]  = mm.m[13];
  r.m[11] = mm.m[14];
  r.m[15] = mm.m[15];
  return r;
}

static inline mat4 translate_mat4( vec3 vv ) {
  mat4 r  = identity_mat4();
  r.m[12] = vv.x;
  r.m[13] = vv.y;
  r.m[14] = vv.z;
  return r;
}

static inline mat4 rot_x_deg_mat4( float deg ) {
  float rad = deg * ONE_DEG_IN_RAD;
  mat4 r    = identity_mat4();
  r.m[5] = r.m[10] = cosf( rad );
  r.m[9]           = -sinf( rad );
  r.m[6]           = sinf( rad );
  return r;
}

static inline mat4 rot_y_deg_mat4( float deg ) {
  float rad = deg * ONE_DEG_IN_RAD;
  mat4 r    = identity_mat4();
  r.m[0] = r.m[10] = cosf( rad );
  r.m[8]           = sinf( rad );
  r.m[2]           = -sinf( rad );
  return r;
}

static inline mat4 rot_z_deg_mat4( float deg ) {
  float rad = deg * ONE_DEG_IN_RAD;
  mat4 r    = identity_mat4();
  r.m[0] = r.m[5] = cosf( rad );
  r.m[4]          = -sinf( rad );
  r.m[1]          = sinf( rad );
  return r;
}

static inline mat4 scale_mat4( vec3 v ) {
  mat4 r  = identity_mat4();
  r.m[0]  = v.x;
  r.m[5]  = v.y;
  r.m[10] = v.z;
  return r;
}

static inline mat4 look_at( vec3 cam_pos, vec3 targ_pos, vec3 up ) {
  mat4 p    = translate_mat4( ( vec3 ){ .x = -cam_pos.x, .y = -cam_pos.y, .z = -cam_pos.z } );
  vec3 d    = sub_vec3_vec3( targ_pos, cam_pos );
  vec3 f    = normalise_vec3( d );
  vec3 r    = normalise_vec3( cross_vec3( f, up ) );
  vec3 u    = normalise_vec3( cross_vec3( r, f ) );
  mat4 ori  = identity_mat4();
  ori.m[0]  = r.x;
  ori.m[4]  = r.y;
  ori.m[8]  = r.z;
  ori.m[1]  = u.x;
  ori.m[5]  = u.y;
  ori.m[9]  = u.z;
  ori.m[2]  = -f.x;
  ori.m[6]  = -f.y;
  ori.m[10] = -f.z;
  return mult_mat4_mat4( ori, p );
}

static inline mat4 perspective( float fovy, float aspect, float near_plane, float far_plane ) {
  float fov_rad = fovy * ONE_DEG_IN_RAD;
  float range   = tanf( fov_rad / 2.0f ) * near_plane;
  float sx      = ( 2.0f * near_plane ) / ( range * aspect + range * aspect );
  float sy      = near_plane / range;
  float sz      = -( far_plane + near_plane ) / ( far_plane - near_plane );
  float pz      = -( 2.0f * far_plane * near_plane ) / ( far_plane - near_plane );
  mat4 m        = { { 0 } };
  m.m[0]        = sx;
  m.m[5]        = sy;
  m.m[10]       = sz;
  m.m[14]       = pz;
  m.m[11]       = -1.0f;
  return m;
}

/* create a standard *asymmetric* perspective projection matrix for special case of a subwindow viewport
- original viewport from (0,0) with size (vp_w, vp_h)
- subwindow viewport from (subvp_x,subvp_y) with size (subvp_w,subvp_h)
- Note: mouse coords, if used, may required a y direction flip.
- Note: this function does not modify near or far plane. It could do my adding z scaling to M.
- Note: this function uses an axis-parallel subwindow but it could be modified to a parallelogram shape.
Code based on excellent problem description here: https://stackoverflow.com/questions/50110934/display-recursively-rendered-scene-into-a-plane
*/
static inline mat4 perspective_offcentre_viewport( int vp_w, int vp_h, int subvp_x, int subvp_y, int subvp_w, int subvp_h, mat4 P_orig ) {
  float subvp_x_ndc = ( (float)subvp_x / (float)vp_w ) * 2.0f - 1.0f;
  float subvp_y_ndc = ( (float)subvp_y / (float)vp_h ) * 2.0f - 1.0f;
  float subvp_w_ndc = ( (float)subvp_w / (float)vp_w ) * 2.0f;
  float subvp_h_ndc = ( (float)subvp_h / (float)vp_h ) * 2.0f;
  // Create a scale and translation transform which maps the range [x_ndc, x_ndc+a_ndc] to [-1,1], and similar for y
  mat4 M  = { { 0 } };
  M.m[0]  = 2.0f / subvp_w_ndc;
  M.m[5]  = 2.0f / subvp_h_ndc;
  M.m[10] = 1.0f;
  M.m[12] = -2.0f * subvp_x_ndc / subvp_w_ndc - 1.0f;
  M.m[13] = -2.0f * subvp_y_ndc / subvp_h_ndc - 1.0f;
  M.m[15] = 1.0f;
  // Pre-Multiply M to the original projection matrix P
  mat4 P_asym = mult_mat4_mat4( M, P_orig );
  return P_asym;
}

/* Takes any world-to-clip space matrix and inverts to retrieve the 8 corner points in world space
based on http://donw.io/post/frustum-point-extraction/

PARAMS
  PVM     - Any world-to-clip space matrix.
  corners - A buffer of 8x vec3. Must not be NULL. World space corner points will be set here.

REMARKS
Expect some floating small point error compared to original world space points.
For plane extraction see https://fgiesen.wordpress.com/2012/08/31/frustum-planes-from-the-projection-matrix/
*/
static inline void frustum_points_from_PV( mat4 PV, vec3* corners_wor ) {
  assert( corners_wor );

  mat4 clip_to_world = inverse_mat4( PV );

  // Start in clip space. First four have -1 in Z for OpenGL. should be 0 for D3D
  vec4 corners_clip[8] = {
    ( vec4 ){ -1, -1, -1, 1 }, // will be nbl 0
    ( vec4 ){ -1, 1, -1, 1 },  // will be ntl 1
    ( vec4 ){ 1, 1, -1, 1 },   // will be ntr 2
    ( vec4 ){ 1, -1, -1, 1 },  // will be nbr 3
    ( vec4 ){ -1, -1, 1, 1 },  // will be fbl 4
    ( vec4 ){ -1, 1, 1, 1 },   // will be ftl 5
    ( vec4 ){ 1, 1, 1, 1 },    // will be ftr 6
    ( vec4 ){ 1, -1, 1, 1 }    // will be fbr 7
  };
  for ( int i = 0; i < 8; i++ ) {
    corners_clip[i] = mult_mat4_vec4( clip_to_world, corners_clip[i] );
    corners_wor[i]  = v3_v4( div_vec4_f( corners_clip[i], corners_clip[i].w ) ); // perspective division
  }
}

/*
PARAMS
  corners_wor - A buffer of 8x vec3, containing corner points in world space. Must not be NULL.
  planes_wor  - A buffer of 6x vec3. Must not be NULL. Bounding plane normals will be set here.
REMARKS
Plane normals point inwards, to be used with frustum test functions */
static inline void frustum_planes_from_points( const vec3* corners_wor, vec3* planes_wor ) {
  assert( corners_wor && planes_wor );
  // edges
  vec3 ftr_m_fbr = sub_vec3_vec3( corners_wor[6], corners_wor[7] ); // a
  vec3 nbr_m_fbr = sub_vec3_vec3( corners_wor[3], corners_wor[7] ); // b
  vec3 ftl_m_fbl = sub_vec3_vec3( corners_wor[5], corners_wor[4] ); // c
  vec3 nbl_m_fbl = sub_vec3_vec3( corners_wor[0], corners_wor[4] ); // d
  vec3 ntr_m_ntl = sub_vec3_vec3( corners_wor[2], corners_wor[1] ); // e
  vec3 ftl_m_ntl = sub_vec3_vec3( corners_wor[5], corners_wor[1] ); // f
  vec3 nbr_m_nbl = sub_vec3_vec3( corners_wor[3], corners_wor[0] ); // g
  vec3 ntl_m_nbl = sub_vec3_vec3( corners_wor[1], corners_wor[0] ); // h
  vec3 fbr_m_fbl = sub_vec3_vec3( corners_wor[7], corners_wor[4] ); // i

  planes_wor[0] = normalise_vec3( cross_vec3( nbr_m_fbr, ftr_m_fbr ) ); // b*a=right (pointing inwards)
  planes_wor[1] = normalise_vec3( cross_vec3( ftl_m_fbl, nbl_m_fbl ) ); // c*d=left
  planes_wor[2] = normalise_vec3( cross_vec3( ftl_m_ntl, ntr_m_ntl ) ); // f*e=top
  planes_wor[3] = normalise_vec3( cross_vec3( nbl_m_fbl, fbr_m_fbl ) ); // d*i=bottom
  planes_wor[4] = normalise_vec3( cross_vec3( ntl_m_nbl, nbr_m_nbl ) ); // h*g=near
  planes_wor[5] = normalise_vec3( cross_vec3( fbr_m_fbl, ftl_m_fbl ) ); // i*c=far
}

static inline versor div_quat_f( versor qq, float s ) { return ( versor ){ .w = qq.w / s, .x = qq.x / s, .y = qq.y / s, .z = qq.z / s }; }

static inline versor mult_quat_f( versor qq, float s ) { return ( versor ){ .w = qq.w * s, .x = qq.x * s, .y = qq.y * s, .z = qq.z * s }; }

// rotates vector v using quaternion q by calculating the sandwich product: v' = qvq^-1
// from pg 89 in E.Lengyel's "FOGED: Mathematics"
// another version (may be faster?):
// t = 2 * cross(q.xyz, v)
// v' = v + q.w * t + cross(q.xyz, t)
// found https://blog.molecular-matters.com/2013/05/24/a-faster-quaternion-vector-multiplication/
// attributed to a post by Fabian Giesen (no longer online)
// TODO(Anton) not tested yet
static inline vec3 mult_quat_vec3( versor q, vec3 v ) {
  vec3 b      = ( vec3 ){ .x = q.x, .y = q.y, .z = q.z };
  float b2    = b.x * b.x + b.y * b.y + b.z * b.z;
  vec3 part_a = mult_vec3_f( v, q.w * q.w - b2 );
  vec3 part_b = mult_vec3_f( b, dot_vec3( v, b ) * 2.0f );
  vec3 part_c = mult_vec3_f( cross_vec3( b, v ), q.w * 2.0f );
  vec3 out    = add_vec3_vec3( part_a, add_vec3_vec3( part_b, part_c ) );
  return out;
}

static inline versor normalise_quat( versor q ) {
  float sum          = q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z;
  const float thresh = 0.0001f;
  if ( fabs( 1.0f - sum ) < thresh ) { return q; }
  float mag = sqrtf( sum );
  return div_quat_f( q, mag );
}

static inline versor mult_quat_quat( versor a, versor b ) {
  versor result;
  result.w = b.w * a.w - b.x * a.x - b.y * a.y - b.z * a.z;
  result.x = b.w * a.x + b.x * a.w - b.y * a.z + b.z * a.y;
  result.y = b.w * a.y + b.x * a.z + b.y * a.w - b.z * a.x;
  result.z = b.w * a.z - b.x * a.y + b.y * a.x + b.z * a.w;
  return normalise_quat( result );
}

static inline versor add_quat_quat( versor a, versor b ) {
  versor result;
  result.w = b.w + a.w;
  result.x = b.x + a.x;
  result.y = b.y + a.y;
  result.z = b.z + a.z;
  return normalise_quat( result );
}

static inline versor quat_from_axis_rad( float radians, vec3 axis ) {
  versor result;
  result.w = cosf( radians / 2.0 );
  result.x = sinf( radians / 2.0 ) * axis.x;
  result.y = sinf( radians / 2.0 ) * axis.y;
  result.z = sinf( radians / 2.0 ) * axis.z;
  return result;
}

static inline versor quat_from_axis_deg( float degrees, vec3 axis ) { return quat_from_axis_rad( ONE_DEG_IN_RAD * degrees, axis ); }

// creates a matrix from a quaternion - use if only needed for rotating a vector then do mult_quat_vec3() instead.
// note: also a function to create a quaternion /from a matrix/: pg 93 in E.Lengyel's "FOGED: Mathematics"
static inline mat4 quat_to_mat4( versor q ) {
  float w = q.w;
  float x = q.x;
  float y = q.y;
  float z = q.z;
  mat4 r;
  r.m[0]  = 1.0f - 2.0f * y * y - 2.0f * z * z;
  r.m[1]  = 2.0f * x * y + 2.0f * w * z;
  r.m[2]  = 2.0f * x * z - 2.0f * w * y;
  r.m[3]  = 0.0f;
  r.m[4]  = 2.0f * x * y - 2.0f * w * z;
  r.m[5]  = 1.0f - 2.0f * x * x - 2.0f * z * z;
  r.m[6]  = 2.0f * y * z + 2.0f * w * x;
  r.m[7]  = 0.0f;
  r.m[8]  = 2.0f * x * z + 2.0f * w * y;
  r.m[9]  = 2.0f * y * z - 2.0f * w * x;
  r.m[10] = 1.0f - 2.0f * x * x - 2.0f * y * y;
  r.m[11] = r.m[12] = r.m[13] = r.m[14] = 0.0f;
  r.m[15]                               = 1.0f;
  return r;
}

static inline float dot_quat( versor q, versor r ) { return q.w * r.w + q.x * r.x + q.y * r.y + q.z * r.z; }

static inline versor slerp_quat( versor q, versor r, float t ) {
  float cos_half_theta = dot_quat( q, r );
  if ( cos_half_theta < 0.0f ) {
    q              = mult_quat_f( q, -1.0f );
    cos_half_theta = dot_quat( q, r );
  }
  if ( fabsf( cos_half_theta ) >= 1.0f ) { return q; }
  float sin_half_theta = sqrtf( 1.0f - cos_half_theta * cos_half_theta );
  versor result;
  if ( fabsf( sin_half_theta ) < 0.001f ) {
    result.w = ( 1.0f - t ) * q.w + t * r.w;
    result.x = ( 1.0f - t ) * q.x + t * r.x;
    result.y = ( 1.0f - t ) * q.y + t * r.y;
    result.z = ( 1.0f - t ) * q.z + t * r.z;
    return result;
  }
  float half_theta = acosf( cos_half_theta );
  float a          = sinf( ( 1.0f - t ) * half_theta ) / sin_half_theta;
  float b          = sinf( t * half_theta ) / sin_half_theta;
  result.w         = q.w * a + r.w * b;
  result.x         = q.x * a + r.x * b;
  result.y         = q.y * a + r.y * b;
  result.z         = q.z * a + r.z * b;
  return result;
}

// [0, 360]
static inline float wrap_degrees_360( float degrees ) {
  if ( degrees >= 0.0f && degrees < 360.0f ) { return degrees; }
  int multiples = (int)( degrees / 360.0f );
  if ( degrees > 0.0f ) {
    degrees = degrees - (float)multiples * 360.0f;
  } else {
    degrees = degrees + (float)multiples * 360.0f;
  }
  return degrees;
}

static inline float abs_diff_btw_degrees( float first, float second ) {
  first  = wrap_degrees_360( first );
  second = wrap_degrees_360( second );

  float diff = fabsf( first - second );
  if ( diff >= 180.0f ) { diff = fabsf( diff - 360.0f ); }
  return diff;
}

// returns t, the distance along the infinite line of the ray from ray origin to intersection.
// If t is negative then intersection is a 'miss' (intersection behind ray origin).
// intersection xyz is then ray_origin + ray_direction * t
static inline float ray_plane( vec3 ray_origin, vec3 ray_direction, vec3 plane_normal, float plane_d ) {
  return -( dot_vec3( ray_origin, plane_normal ) + plane_d ) / dot_vec3( ray_direction, plane_normal );
}

// adapted from https://psgraphics.blogspot.com/2016/02/new-simple-ray-box-test-from-andrew.html
static inline bool ray_aabb( vec3 ray_origin, vec3 ray_direction, vec3 aabb_min, vec3 aabb_max, float tmin, float tmax ) {
  float* rd      = &ray_direction.x;
  float* ro      = &ray_origin.x;
  float* box_min = &aabb_min.x;
  float* box_max = &aabb_max.x;
  for ( int i = 0; i < 3; i++ ) {
    float invD = 1.0f / rd[i];
    float t0   = ( box_min[i] - ro[i] ) * invD;
    float t1   = ( box_max[i] - ro[i] ) * invD;
    if ( invD < 0.0 ) {
      float tmp = t0;
      t0        = t1;
      t1        = tmp;
    }
    tmin = t0 > tmin ? t0 : tmin;
    tmax = t1 < tmax ? t1 : tmax;
    if ( tmax <= tmin ) { return false; }
  }
  return true;
}

typedef struct obb_t {   // A in RTR notation
  vec3 centre;           // a^c in RTR notation
  vec3 norm_side_dir[3]; // a^u, a^v, a^w in RTR notation
  float half_lengths[3]; // centre to face. must be positive. h_u, h_v, h_w in RTR notation
} obb_t;

// t is intersection distance along ray
// face_num is the slab index (1,2,3) corresponding to box side direction intersected. face_num will be negative for the opposing side.
// note that it's not (0,1,2) because negative zero for the opposing face would be problematic
static inline bool ray_obb( obb_t box, vec3 ray_o, vec3 ray_d, float* t, int* face_num ) {
  assert( t );
  *t         = 0.0f;
  float tmin = -INFINITY;
  float tmax = INFINITY;
  int imin = 0, imax = 0;
  vec3 p = sub_vec3_vec3( box.centre, ray_o );
  for ( int i = 0; i < 3; i++ ) { // 3 "slabs" (pair of front/back planes)
    float e = dot_vec3( box.norm_side_dir[i], p );
    float f = dot_vec3( box.norm_side_dir[i], ray_d );
    if ( fabs( f ) > FLT_EPSILON ) {
      float t1 = ( e + box.half_lengths[i] ) / f; // intersection on front
      float t2 = ( e - box.half_lengths[i] ) / f; // and back side of slab
      if ( t1 > t2 ) {
        float tmp = t1;
        t1        = t2;
        t2        = tmp;
      }
      if ( t1 > tmin ) {
        tmin = t1;
        imin = i;
      }
      if ( t2 < tmax ) {
        tmax = t2;
        imax = -i;
      }
      if ( tmin > tmax ) { return false; }
      if ( tmax < 0 ) { return false; }
    } else if ( -e - box.half_lengths[i] > 0 || -e + box.half_lengths[i] < 0 ) {
      return false;
    }
  }
  *t        = tmin > 0 ? tmin : tmax;
  *face_num = tmin > 0 ? imin + 1 : imax + 1;
  return true;
}

#ifdef __cplusplus
}
#endif

/*
-------------------------------------------------------------------------------------
This software is available under two licences - you may use it under either licence.
-------------------------------------------------------------------------------------
FIRST LICENCE OPTION

>                                  Apache License
>                            Version 2.0, January 2004
>                         http://www.apache.org/licenses/
>    Copyright 2019 Anton Gerdelan.
>    Licensed under the Apache License, Version 2.0 (the "License");
>    you may not use this file except in compliance with the License.
>    You may obtain a copy of the License at
>        http://www.apache.org/licenses/LICENSE-2.0
>    Unless required by applicable law or agreed to in writing, software
>    distributed under the License is distributed on an "AS IS" BASIS,
>    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
>    See the License for the specific language governing permissions and
>    limitations under the License.
-------------------------------------------------------------------------------------
SECOND LICENCE OPTION

> This is free and unencumbered software released into the public domain.
>
> Anyone is free to copy, modify, publish, use, compile, sell, or
> distribute this software, either in source code form or as a compiled
> binary, for any purpose, commercial or non-commercial, and by any
> means.
>
> In jurisdictions that recognize copyright laws, the author or authors
> of this software dedicate any and all copyright interest in the
> software to the public domain. We make this dedication for the benefit
> of the public at large and to the detriment of our heirs and
> successors. We intend this dedication to be an overt act of
> relinquishment in perpetuity of all present and future rights to this
> software under copyright law.
>
> THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
> EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
> MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
> IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
> OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
> ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
> OTHER DEALINGS IN THE SOFTWARE.
>
> For more information, please refer to <http://unlicense.org>
-------------------------------------------------------------------------------------
*/
