#pragma once
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "assert.h"
#include <algorithm>
#include <numbers>
#include "MyMath.h"

enum class LRDirecion {
	kright,
	kLeft,
};

struct CollisionMapInfo {

	bool ceiling = false; // 天井衝突
	bool landing = false; // 着地
	bool hitWall = false; // 壁接触
	Vector3 move;         // 移動量
};

enum Corner {
	kRightBottom,
	kLeftBottom,
	kRightTop,
	kLeftTop,
	kNumCorner // 要素数
};


class Enemy;
class MapChipField;
class Player {

public:
	// 初期化
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	// 更新
	void Update();

	// 描画
	void Draw();

	void PrayerMove(); // 自機の動き
	void PrayerTurn(); // 自機の振り向き

	float EaseOutSine(float x);
	const WorldTransform& GetWorldTransform() { return worldTransform_; }
	const Vector3& GetVelocity() const { return velocity_; }
	void SetMapChipField(MapChipField* mapChipFild) { mapChipFild_ = mapChipFild; }

	// map衝突判定
	void MapCollision(CollisionMapInfo& info);
	Vector3 CornerPosition(const Vector3& centor, Corner corner);
	void PlayerCollisionMove(const CollisionMapInfo& inffo);
	void CeilingCollisionMove(const CollisionMapInfo& info);
	void OnGroundSwitching(const CollisionMapInfo& info);
	void HitWallCollisionMove(const CollisionMapInfo& info);

	// 当たり判定
	void CollisionMapInfoTop(CollisionMapInfo& info);
	void CollisionMapInfoBootm(CollisionMapInfo& info);
	void CollisionMapInfoRight(CollisionMapInfo& info);
	void CollisionMapInfoLeft(CollisionMapInfo& info);

	Vector3 GetWorldPosition();
	AABB GetAABB();
	void OnCollision(const Enemy*enemy);

	bool GetIsDead_() const { return isDead_; }

private:
	WorldTransform worldTransform_;            // ワールド変換データ
	Model* model_ = nullptr;                   // モデル
	ViewProjection* viewProjection_ = nullptr; // ViewProjection
	// 移動
	Vector3 velocity_ = {};                          // 速度
	static inline const float kAccleration = 0.01f;  // 定数加速度
	static inline const float kAttenuation = 0.2f;   // 速度減衰率
	static inline const float kLimitRunSpeed = 1.0f; // 最大速度制限
	// 振り向き
	LRDirecion lrDirection_ = LRDirecion::kright;
	float turnFirstRotationY_ = 0.0f;           // 現在の向き
	float turnTimer_ = 0.0f;                    // 振り向き時間
	static inline const float KtimeTurn = 0.5f; // 角度補間タイム
	// ジャンプ
	bool onGround_ = true;                                 // 接点状態フラグ
	static inline const float kGravityAccleration = 0.05f; // 重力加速度
	static inline const float kLimitFallSpeed = 1.0f;      // 最大落下速度
	static inline const float kJampAcceleration = 0.5f;    // ジャンプ初速
	// 当たり判定
	MapChipField* mapChipFild_ = nullptr;
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;
	static inline const float kBlank = 1.0;
	static inline const float kAttenuationLanding = 0.1f;
	static inline const float kCollisionsmallnumber = 0.1f;
	static inline const float kAttenuationWall = 0.1f;

	//死んだ
	bool isDead_ = false;
};
