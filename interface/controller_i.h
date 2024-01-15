#pragma once
#include <cstdint>

#ifdef DOF9MON_EXPORTS
#  define DOF9MON_API __declspec(dllexport)
#else
#  define DOF9MON_API __declspec(dllimport)
#endif

//////////////////////////////////////////////////////////////////////////
// Լ����
//  1. ʵʱ����ֻ̨����һ��������ӣ����������ӻᱻ������ֱ����ǰ���ӶϿ���
//  2. �����9���ɶ�һ���ֻ�й���1�������6+3���ɶȣ�����1��ʾ6���ɶȣ�����2��ʾ3���ɶȡ�
//  3. ���е�ʱ�궼�� clock ��ʾ����ʵʱ����̨������������ʱ�Ӽ��������Բ������ǿ���̨ʱ�䡣����̨Ŀǰ�Ĳ����� 8ms��10ms
//  4. ָ��Ϸ��Լ����ʵʱ����̨ʵ�֣��ͻ��������ظ���ɡ��磺���ⲿ����ģʽ�£��ͻ�����move����������ģ�����̨���Զ����ԡ�
//  5. ���κ�����£������Ҫ����ͣ�����ͽ���ȫģʽ��
//////////////////////////////////////////////////////////////////////////

namespace dof9
{

/// ʵʱ����̨�Ĺ���ģʽ
/// ����Ϊ��������ģʽ������ģʽ֮���޷��л��������԰�ȫģʽΪ��ת
enum WorkMode
{
    invalid_mode = 0,   // ��Чģʽ(���ǳ�����󣬷��򲻻���ָ�ģʽ)
    manual_mode,        // �ֶ�ģʽ
    playback_mode,      // �ط�ģʽ
    dyn_sim_mode,       // ����ѧ����ģʽ
    sin_wave_mode,      // ��������ģʽ
    extern_ctrl_mode    // �ⲿ����ģʽ
};


//////////////////////////////////////////////////////////////////////////
// ʵʱ����̨��״̬��Ϣ �����κ�ģʽ�£�ÿ��ʱ����������ֻ��1����
//////////////////////////////////////////////////////////////////////////
struct SimState
{
    WorkMode    work_mode;  ///< ����ģʽ (��ǰģʽ����밲ȫģʽǰ��ģʽ)
    uint32_t    clock;      ///< ʱ��
    float       clock_delay;///< ��ǰʱ���ӳ�(��)
    float       pos_cmd[9]; ///< ��ǰʱ�ӷ���ת̨λ��ָ��
    float       pos_stat[9];///< ��ǰʱ���յ�ת̨��λ��״̬

    bool limit_protect_on;  ///< ����λ��������
    bool safe_mode;         ///< �Ƿ��ڰ�ȫģʽ

    bool rfm1_write_on;     ///< ����1д����
    bool rfm1_comm_err;     ///< ����1ͨѶʧ��
    bool rfm1_write_err;    ///< ����1����ʧ��
    bool rfm1_read_err;     ///< ����1����ʧ��
    bool rfm2_write_on;     ///< ����2..
    bool rfm2_comm_err;     ///< ...
    bool rfm2_write_err;    ///< ...
    bool rfm2_read_err;     ///< ...

    bool limit_err[9];      ///< 9�����ɶ��Ƿ���λ��״̬��ʶ
    bool moving_state[9];   ///< 9�����ɶ��Ƿ������˶���״̬��ʶ
};


//////////////////////////////////////////////////////////////////////////
/// ʵʱ����̨�ļ��ӽӿ�, �ɿͻ�������ʵ��
//////////////////////////////////////////////////////////////////////////
struct Monitor
{
    /// ��Ϣ����
    enum MsgLevel { lvl_msg, lvl_warn, lvl_err };

    /// �յ�����״̬���ݰ�ʱ����
    virtual void onRecvState(const SimState& stat) = 0;

    /// �յ��ı���Ϣʱ���á�����̨�������κ�ʱ�䷢���ı������Ϣ�����������������
    ///@clock : ��Ϣ������ʱ��
    virtual void onRecvMsg(uint32_t clock, MsgLevel lvl, const char* msg) = 0;

    /// �ӿڿ�����������ʱ���ã�������ʧ��
    virtual void onError(const char* msg) = 0;
};


//////////////////////////////////////////////////////////////////////////
/// ʵʱ����̨�ӿ�, �ͻ�ͨ���˽ӿ�����������̨
//////////////////////////////////////////////////////////////////////////
struct Controller
{
    /// ���ٶ����൱�� delete
    virtual void release() = 0;

    /// ��ѯ�Ƿ�����
    virtual bool isConnected() = 0;

    /// ����д���˿��������6+3�����ɶȣ�3���ɶ�ʹ�� rfm2
    ///@return : ָ���Ƿ��ͳɹ�(��ͬ)
    virtual bool enableWriteRfm(bool on) = 0;
    virtual bool enableWriteRfm2(bool on) = 0;

    /// ���밲ȫģʽ
    /// ���κ�����¶����Խ��밲ȫģʽ����ʱת̨���Զ�����ͣ��.
    /// ����Ѿ����ڰ�ȫģʽ������Ը�ָ��
    virtual bool enterSafeMode() = 0;

    /// ��������ģʽ
    /// ֻ�ܴӰ�ȫģʽ���룬��ͼ������ģʽ������һ������ģʽ��ָ�������
    virtual bool enterNormalMode(WorkMode mode) = 0;

    /// ͬʱ����������ɶȣ��Ѿ���������״̬�����ɶȽ�������Ӧ
    ///@mask : ���ɶ�����, ���λ��ʾ�� 1 �����ɶ�
    ///@v : Ҫ�ƶ�����Ŀ��λ��
    virtual bool move(uint32_t mask, float v[9]) = 0;
    
    /// ����ĳ�����ɶȣ�����Ӱ���������е����ɶ�
    ///@index : Ҫ�˶������ɶȣ�0 ��ʾ��1�����ɶ�
    ///@v : Ŀ��λ��
    virtual bool move(uint32_t index, float v) = 0;

    /// ͣ�������Ծ�ϸ�ؿ���ÿ�����ɶȡ�ֻ���ֶ�ģʽ����Ч������ģʽ�º��Ը�ָ�
    ///@mask : ���ɶ����룬-1 ��ʾȫ�����ɶ�
    virtual bool stop(uint32_t mask) = 0;

    /// ����ת̨�˶����Ʊ�־(6���ɶ�ת̨��Ҫ���øñ�־��������)
    virtual bool setMoveCtrlFlag(bool on) = 0;
};


/// ��ʵʱ����̨����ͨ������
///@monitor [in] : ������ָ�룬���ӽ��������������¼�����ص��ýӿڵĺ���
///@ip [in] : ����̨�� ip ��ַ
///@port [in] : ����̨�Ķ˿�
///@local_ip [in] : ���� ip ��ַ, ��Ϊ�� 
///@return : ����̨�ӿڵ�ָ�룬����֮��ͨ�� release �����ͷ�
DOF9MON_API Controller* newConnection(Monitor* monitor, const char* ip, uint16_t port, const char* local_ip = "");

} //namespace dof9