#include "adrc.h"

namespace adrc
{

adrc_controller::adrc_controller()
{
  adrc_hdr_ = std::make_shared<adrc_secondorder>(0.03,2.0);
}

adrc_controller::~adrc_controller()
{

}

void adrc_controller::SetTrack(const std::vector<Pose<float> > &path, const float &current_vel)
{
  track_.DealWithPath(path,current_vel);
}

void adrc_controller::ClearTrack()
{
  track_.ClearSpline();
}

VectorX2<float> adrc_controller::RunAdrc(const State &x, const float &delta)
{
  //1.如果判断机器人已经超过目标位置，则需要返回零值
  VectorX2<float> v1;
  if(track_.IsGoReached(x))
  {
    v1.x = 0.0;
    v1.y = 0.0;
    //std::cout<<"已经超过目标位置，直接返回零值"<<std::endl;
    return v1;
  }
  //2.计算机器人垂足点,获取预瞄点坐标
  mpc::SplineParam spline = track_.GetTrack();
  int current_point = track_.CalculateNearestIndex(x);
  int max_point     = spline.x.size() - 1;
  int point         = current_point + 60;
  int point1        = current_point + 20;//最后20个点，全力调整角速度
  mpc::clamp(point,0,max_point);
  mpc::clamp(point1,0,max_point);
  float tx_in_global   = spline.x.at(point);
  float ty_in_global   = spline.y.at(point);
  float tyaw_in_global = spline.yaw.at(point);
  controller::MessageToRos::GetInstance()->VisualTargetPoint(tx_in_global,ty_in_global,tyaw_in_global);
  //2.根据预瞄点计算
  //3.将目标点转化到机器人坐标系下
  float tx_in_robot   =  tx_in_global*cos(x.yaw) + ty_in_global*sin(x.yaw);
  float ty_in_robot   = -tx_in_global*sin(x.yaw) + ty_in_global*cos(x.yaw);
  float tyaw_in_robot =  tyaw_in_global - x.yaw;
  //4.执行自抗扰控制,先求解出vx,vy,yaw,再转化为车头偏向角
  float vx      = x.v;//记录当前线速度，方便后期进行车头偏向角的转化
  float vy      = adrc_hdr_->ComputeControl(0.0,ty_in_robot);
  //float vtheta  = adrc_hdr_->ComputeControl(0.0,tyaw_in_robot);
  std::cout<<"ty_in_robot:"<<ty_in_robot<<"tyaw_in_robot:"<<tyaw_in_robot<<std::endl;
  std::cout<<"vy:"<<vy<<std::endl;
  //5.输出量计算,对横向偏差的消除，需要转化为等效的角度偏差
  v1.x = 0.0;
  v1.y = 0.0;
  return v1;
}

}
