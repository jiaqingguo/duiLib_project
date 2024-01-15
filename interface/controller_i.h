#pragma once
#include <cstdint>

#ifdef DOF9MON_EXPORTS
#  define DOF9MON_API __declspec(dllexport)
#else
#  define DOF9MON_API __declspec(dllimport)
#endif

//////////////////////////////////////////////////////////////////////////
// 约定：
//  1. 实时控制台只接受一个监控连接，其它的连接会被阻塞，直到当前连接断开。
//  2. 如果是9自由度一体就只有光纤1，如果是6+3自由度，光纤1表示6自由度，光纤2表示3自由度。
//  3. 所有的时标都用 clock 表示，即实时控制台自启动以来的时钟计数，乘以步长就是控制台时间。控制台目前的步长有 8ms、10ms
//  4. 指令合法性检查由实时控制台实现，客户端无需重复完成。如：在外部控制模式下，客户发出move操作是允许的，控制台会自动忽略。
//  5. 在任何情况下，如果需要紧急停车，就进安全模式。
//////////////////////////////////////////////////////////////////////////

namespace dof9
{

/// 实时控制台的工作模式
/// 以下为正常工作模式，正常模式之间无法切换，必须以安全模式为中转
enum WorkMode
{
    invalid_mode = 0,   // 无效模式(除非程序错误，否则不会出现该模式)
    manual_mode,        // 手动模式
    playback_mode,      // 回放模式
    dyn_sim_mode,       // 动力学仿真模式
    sin_wave_mode,      // 正弦曲线模式
    extern_ctrl_mode    // 外部控制模式
};


//////////////////////////////////////////////////////////////////////////
// 实时控制台的状态信息 （在任何模式下，每个时钟周期有且只有1包）
//////////////////////////////////////////////////////////////////////////
struct SimState
{
    WorkMode    work_mode;  ///< 工作模式 (当前模式或进入安全模式前的模式)
    uint32_t    clock;      ///< 时标
    float       clock_delay;///< 当前时钟延迟(秒)
    float       pos_cmd[9]; ///< 当前时钟发给转台位置指令
    float       pos_stat[9];///< 当前时钟收到转台的位置状态

    bool limit_protect_on;  ///< 超限位保护开启
    bool safe_mode;         ///< 是否处于安全模式

    bool rfm1_write_on;     ///< 光纤1写开启
    bool rfm1_comm_err;     ///< 光纤1通讯失败
    bool rfm1_write_err;    ///< 光纤1收数失败
    bool rfm1_read_err;     ///< 光纤1发数失败
    bool rfm2_write_on;     ///< 光纤2..
    bool rfm2_comm_err;     ///< ...
    bool rfm2_write_err;    ///< ...
    bool rfm2_read_err;     ///< ...

    bool limit_err[9];      ///< 9个自由度是否超限位的状态标识
    bool moving_state[9];   ///< 9个自由度是否正在运动的状态标识
};


//////////////////////////////////////////////////////////////////////////
/// 实时控制台的监视接口, 由客户界面来实现
//////////////////////////////////////////////////////////////////////////
struct Monitor
{
    /// 消息级别
    enum MsgLevel { lvl_msg, lvl_warn, lvl_err };

    /// 收到仿真状态数据包时调用
    virtual void onRecvState(const SimState& stat) = 0;

    /// 收到文本消息时调用。控制台可能在任何时间发送文本类的消息，如操作或错误的详情
    ///@clock : 消息发生的时标
    virtual void onRecvMsg(uint32_t clock, MsgLevel lvl, const char* msg) = 0;

    /// 接口库自身发生错误时调用，如连接失败
    virtual void onError(const char* msg) = 0;
};


//////////////////////////////////////////////////////////////////////////
/// 实时控制台接口, 客户通过此接口来驱动控制台
//////////////////////////////////////////////////////////////////////////
struct Controller
{
    /// 销毁对象，相当于 delete
    virtual void release() = 0;

    /// 查询是否连接
    virtual bool isConnected() = 0;

    /// 启用写光纤卡。如果是6+3个自由度，3自由度使用 rfm2
    ///@return : 指令是否发送成功(下同)
    virtual bool enableWriteRfm(bool on) = 0;
    virtual bool enableWriteRfm2(bool on) = 0;

    /// 进入安全模式
    /// 在任何情况下都可以进入安全模式，此时转台会自动减速停车.
    /// 如果已经处于安全模式，则忽略该指令
    virtual bool enterSafeMode() = 0;

    /// 进入正常模式
    /// 只能从安全模式进入，试图从正常模式进入另一个正常模式，指令将被忽略
    virtual bool enterNormalMode(WorkMode mode) = 0;

    /// 同时启动多个自由度，已经处于运行状态的自由度将不受响应
    ///@mask : 自由度掩码, 最低位表示第 1 个自由度
    ///@v : 要移动到的目标位置
    virtual bool move(uint32_t mask, float v[9]) = 0;
    
    /// 启动某个自由度，不会影响正在运行的自由度
    ///@index : 要运动的自由度，0 表示第1个自由度
    ///@v : 目标位置
    virtual bool move(uint32_t index, float v) = 0;

    /// 停车，可以精细地控制每个自由度。只在手动模式下有效，其它模式下忽略该指令。
    ///@mask : 自由度掩码，-1 表示全部自由度
    virtual bool stop(uint32_t mask) = 0;

    /// 设置转台运动控制标志(6自由度转台需要设置该标志才能驱动)
    virtual bool setMoveCtrlFlag(bool on) = 0;
};


/// 与实时控制台建立通信连接
///@monitor [in] : 监视器指针，连接建立后发生的所有事件都会回调该接口的函数
///@ip [in] : 控制台的 ip 地址
///@port [in] : 控制台的端口
///@local_ip [in] : 本机 ip 地址, 可为空 
///@return : 控制台接口的指针，用完之后通过 release 函数释放
DOF9MON_API Controller* newConnection(Monitor* monitor, const char* ip, uint16_t port, const char* local_ip = "");

} //namespace dof9