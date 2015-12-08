#ifndef _DEVICE_H_
#define _DEVICE_H_
#include "xtypes.h"

/**
 * エミュレータで使用するデバイスの抽象
 */
class IDevice {
public:
  /**
   * デバイスのリセット
   *
   * <P>
   * このメソッドの呼び出し後，このデバイスはどのようなメソッドの呼び
   * 出しに対しても，実行時エラーを起こしてはならない．逆に，このメソッ
   * ドを呼ぶ以前は，他のメソッドの動作は一切保証しなくても良い。
   * </P>
   */
  virtual void Reset() = 0;

  /**
   * デバイスへの書き込み
   *
   * @param adr アドレス
   * @param val 書き込む値
   * @param id  デバイス識別情報．一つのデバイスが複数のIOをサポートする時など
   * @return 成功時 true 失敗時 false
   */
  virtual bool Write(UINT32 adr, UINT32 val, UINT32 id = 0) = 0;

  /**
   * デバイスから読み込み
   *
   * @param adr アドレス
   * @param val 読み出した値を受け取る変数．
   * @return 成功時 true 失敗時 false
   */
  virtual bool Read(UINT32 adr, UINT32 &val, UINT32 id = 0) = 0;

  /**
   * 各種オプションを設定する(もしあれば)
   */
  virtual void SetOption(int id, int val){};
};

#endif
