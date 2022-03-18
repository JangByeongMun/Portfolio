#pragma once

// 설명 : 죽은 이유를 표시해주는창
class DeadReasonUI
{
public:
	// constrcuter destructer
	DeadReasonUI();
	~DeadReasonUI();

	// delete Function
	DeadReasonUI(const DeadReasonUI& _Other) = delete;
	DeadReasonUI(DeadReasonUI&& _Other) noexcept = delete;
	DeadReasonUI& operator=(const DeadReasonUI& _Other) = delete;
	DeadReasonUI& operator=(DeadReasonUI&& _Other) noexcept = delete;

protected:

private:

};

