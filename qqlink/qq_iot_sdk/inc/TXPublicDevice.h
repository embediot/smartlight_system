#ifndef __TX_PUBLIC_DEVICE_H_
#define __TX_PUBLIC_DEVICE_H_

#include "TXSDKCommonDef.h"

CXX_EXTERN_BEGIN

//        公共设备相关接口


///////////////////////////////////////////////////////////////////////////
//
//                          【向QQ服务号发送模板消息】
//
///////////////////////////////////////////////////////////////////////////
//
//   模板消息用于公共设备向QQ服务号发送消息，进而向用户通知公共设备的服务信息
//   模板消息是一组JSON格式的数据包
//   公共设备必须按照一定的JSON格式模板，填写数据，才能成功的将模板消息送达QQ服务号
//

//callback: 用于确认消息是否送达了QQ物联后台服务器
typedef void (*on_send_template_msg_ret)(unsigned int cookie, int err_code);

/**
 * 接口说明：向QQ服务号发送模板消息
 * 参数说明：
 *  msg_data                  消息内容，JSON格式的字符串，以\0结尾。
 *  ret_callback            发送结果回调
 *  cookie                  由sdk内部分配唯一值，并会在发送结果回调时回传，用于唯一标识当前调用
 */
SDK_API void tx_send_template_msg(char *msg_data, on_send_template_msg_ret ret_callback, unsigned int *cookie);


CXX_EXTERN_END


#endif /* __TX_PUBLIC_DEVICE_H_ */
