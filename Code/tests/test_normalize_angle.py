"""
单元测试 - Normalize_Angle 函数

测试覆盖：
1. 正常输入场景
2. 边界值（0, 2π, -2π, π 等）
3. 负数输入
4. 大于 2π 的输入
5. 小于 0 的输入
6. 极端值
"""

import math
import pytest


# Normalize_Angle 函数的 Python 重实现
def normalize_angle(angle_rad: float) -> float:
    """
    将角度归一化到 [0, 2π) 范围内
    
    Args:
        angle_rad: 输入角度（弧度）
    
    Returns:
        归一化后的角度，范围 [0, 2π)
    """
    a = math.fmod(angle_rad, 2.0 * math.pi)
    if a >= 0.0:
        return a
    else:
        return a + 2.0 * math.pi


class TestNormalizeAngle:
    """Normalize_Angle 函数的单元测试类"""
    
    # ==================== 正常输入场景 ====================
    
    def test_positive_angle_zero(self):
        """测试输入为 0 的情况"""
        angle = 0.0
        result = normalize_angle(angle)
        assert result == pytest.approx(0.0)
        assert 0.0 <= result < 2.0 * math.pi
    
    def test_positive_angle_small_positive(self):
        """测试输入为小的正数"""
        angle = math.pi / 4  # 45度
        result = normalize_angle(angle)
        expected = math.pi / 4
        assert result == pytest.approx(expected)
        assert 0.0 <= result < 2.0 * math.pi
    
    def test_positive_angle_pi(self):
        """测试输入为 π 的情况"""
        angle = math.pi  # 180度
        result = normalize_angle(angle)
        expected = math.pi
        assert result == pytest.approx(expected)
        assert 0.0 <= result < 2.0 * math.pi
    
    def test_positive_angle_half_pi(self):
        """测试输入为 π/2 的情况"""
        angle = math.pi / 2  # 90度
        result = normalize_angle(angle)
        expected = math.pi / 2
        assert result == pytest.approx(expected)
        assert 0.0 <= result < 2.0 * math.pi
    
    def test_positive_angle_three_half_pi(self):
        """测试输入为 3π/2 的情况"""
        angle = 3.0 * math.pi / 2  # 270度
        result = normalize_angle(angle)
        expected = 3.0 * math.pi / 2
        assert result == pytest.approx(expected)
        assert 0.0 <= result < 2.0 * math.pi
    
    # ==================== 边界值测试 ====================
    
    def test_boundary_positive_two_pi(self):
        """测试输入正好 的情况（边界值）"""
       为 2π angle = 2.0 * math.pi  # 360度
        result = normalize_angle(angle)
        # fmod(2π, 2π) = 0，所以结果应该是 0
        assert result == pytest.approx(0.0)
        assert 0.0 <= result < 2.0 * math.pi
    
    def test_boundary_negative_two_pi(self):
        """测试输入正好为 -2π 的情况"""
        angle = -2.0 * math.pi  # -360度
        result = normalize_angle(angle)
        # fmod(-2π, 2π) = 0，结果应该是 0
        assert result == pytest.approx(0.0)
        assert 0.0 <= result < 2.0 * math.pi
    
    def test_boundary_negative_epsilon(self):
        """测试输入为接近 0 的负数（-ε）"""
        epsilon = 1e-10
        angle = -epsilon
        result = normalize_angle(angle)
        expected = 2.0 * math.pi - epsilon
        assert result == pytest.approx(expected, rel=1e-9, abs=1e-12)
        assert 0.0 <= result < 2.0 * math.pi
    
    def test_boundary_positive_epsilon(self):
        """测试输入为接近 0 的正数（+ε）"""
        epsilon = 1e-10
        angle = epsilon
        result = normalize_angle(angle)
        expected = epsilon
        assert result == pytest.approx(expected, rel=1e-9, abs=1e-12)
        assert 0.0 <= result < 2.0 * math.pi
    
    def test_boundary_slightly_less_than_two_pi(self):
        """测试输入略小于 2π"""
        angle = 2.0 * math.pi - 1e-10
        result = normalize_angle(angle)
        expected = 2.0 * math.pi - 1e-10
        assert result == pytest.approx(expected, rel=1e-9, abs=1e-12)
        assert 0.0 <= result < 2.0 * math.pi
    
    def test_boundary_slightly_greater_than_two_pi(self):
        """测试输入略大于 2π"""
        angle = 2.0 * math.pi + 1e-10
        result = normalize_angle(angle)
        expected = 1e-10  # 超出部分取模
        assert result == pytest.approx(expected, rel=1e-9, abs=1e-12)
        assert 0.0 <= result < 2.0 * math.pi
    
    # ==================== 负数输入测试 ====================
    
    def test_negative_angle_pi(self):
        """测试输入为 -π 的情况"""
        angle = -math.pi  # -180度
        result = normalize_angle(angle)
        expected = math.pi  # 应该等于 π
        assert result == pytest.approx(expected)
        assert 0.0 <= result < 2.0 * math.pi
    
    def test_negative_angle_half_pi(self):
        """测试输入为 -π/2 的情况"""
        angle = -math.pi / 2  # -90度
        result = normalize_angle(angle)
        expected = 3.0 * math.pi / 2  # 应该等于 270度
        assert result == pytest.approx(expected)
        assert 0.0 <= result < 2.0 * math.pi
    
    def test_negative_angle_small(self):
        """测试输入为小负数"""
        angle = -math.pi / 4  # -45度
        result = normalize_angle(angle)
        expected = 7.0 * math.pi / 4  # 应该等于 315度
        assert result == pytest.approx(expected)
        assert 0.0 <= result < 2.0 * math.pi
    
    def test_negative_angle_very_small(self):
        """测试输入为非常小的负数"""
        angle = -1e-15
        result = normalize_angle(angle)
        expected = 2.0 * math.pi - 1e-15
        assert result == pytest.approx(expected, rel=1e-9, abs=1e-12)
        assert 0.0 <= result < 2.0 * math.pi
    
    def test_negative_angle_minus_three_pi_half(self):
        """测试输入为 -3π/2 的情况"""
        angle = -3.0 * math.pi / 2  # -270度
        result = normalize_angle(angle)
        expected = math.pi / 2  # 应该等于 90度
        assert result == pytest.approx(expected)
        assert 0.0 <= result < 2.0 * math.pi
    
    # ==================== 大于 2π 的输入测试 ====================
    
    def test_angle_greater_than_two_pi(self):
        """测试输入大于 2π"""
        angle = 3.0 * math.pi  # 540度 (1.5圈)
        result = normalize_angle(angle)
        expected = math.pi  # 180度
        assert result == pytest.approx(expected)
        assert 0.0 <= result < 2.0 * math.pi
    
    def test_angle_four_pi(self):
        """测试输入为 4π（2圈）"""
        angle = 4.0 * math.pi
        result = normalize_angle(angle)
        assert result == pytest.approx(0.0)
        assert 0.0 <= result < 2.0 * math.pi
    
    def test_angle_five_pi(self):
        """测试输入为 5π（2.5圈）"""
        angle = 5.0 * math.pi
        result = normalize_angle(angle)
        expected = math.pi
        assert result == pytest.approx(expected)
        assert 0.0 <= result < 2.0 * math.pi
    
    def test_angle_slightly_greater_than_multiple_of_two_pi(self):
        """测试输入略大于 2π 的整数倍"""
        angle = 4.0 * math.pi + 0.001
        result = normalize_angle(angle)
        expected = 0.001
        assert result == pytest.approx(expected, rel=1e-9)
        assert 0.0 <= result < 2.0 * math.pi
    
    # ==================== 负数且大于 2π 的输入测试 ====================
    
    def test_negative_angle_greater_than_two_pi(self):
        """测试输入为负数且绝对值大于 2π"""
        angle = -5.0 * math.pi  # -900度
        result = normalize_angle(angle)
        expected = math.pi  # -5π ≡ π (mod 2π)
        assert result == pytest.approx(expected)
        assert 0.0 <= result < 2.0 * math.pi
    
    def test_negative_angle_minus_four_pi(self):
        """测试输入为 -4π（-2圈）"""
        angle = -4.0 * math.pi
        result = normalize_angle(angle)
        assert result == pytest.approx(0.0)
        assert 0.0 <= result < 2.0 * math.pi
    
    # ==================== 极端值测试 ====================
    
    def test_extreme_large_positive(self):
        """测试非常大的正数"""
        angle = 1e12 * math.pi  # 极大的正数
        result = normalize_angle(angle)
        # 结果应该在 [0, 2π) 范围内
        assert 0.0 <= result < 2.0 * math.pi
    
    def test_extreme_large_negative(self):
        """测试非常大的负数"""
        angle = -1e12 * math.pi  # 极大的负数
        result = normalize_angle(angle)
        # 结果应该在 [0, 2π) 范围内
        assert 0.0 <= result < 2.0 * math.pi
    
    def test_extreme_small_positive(self):
        """测试极小的正数"""
        angle = 1e-100
        result = normalize_angle(angle)
        assert result == pytest.approx(angle)
        assert 0.0 <= result < 2.0 * math.pi
    
    def test_extreme_small_negative(self):
        """测试极小的负数"""
        angle = -1e-100
        result = normalize_angle(angle)
        expected = 2.0 * math.pi - 1e-100
        assert result == pytest.approx(expected, rel=1e-9, abs=1e-100)
        assert 0.0 <= result < 2.0 * math.pi
    
    # ==================== 特殊值测试 ====================
    
    def test_special_value_e(self):
        """测试使用自然常数 e"""
        angle = math.e
        result = normalize_angle(angle)
        assert 0.0 <= result < 2.0 * math.pi
    
    def test_special_value_golden_ratio(self):
        """测试使用黄金分割比例"""
        golden_ratio = (1.0 + math.sqrt(5.0)) / 2.0
        angle = golden_ratio * math.pi
        result = normalize_angle(angle)
        assert 0.0 <= result < 2.0 * math.pi
    
    def test_special_value_sqrt_two(self):
        """测试使用 sqrt(2)"""
        angle = math.sqrt(2.0) * math.pi
        result = normalize_angle(angle)
        assert 0.0 <= result < 2.0 * math.pi
    
    # ==================== 整数圈数测试 ====================
    
    def test_one_full_rotation(self):
        """测试输入为 2π（一圈）"""
        angle = 2.0 * math.pi
        result = normalize_angle(angle)
        assert result == pytest.approx(0.0)
        assert 0.0 <= result < 2.0 * math.pi
    
    def test_two_full_rotations(self):
        """测试输入为 4π（两圈）"""
        angle = 4.0 * math.pi
        result = normalize_angle(angle)
        assert result == pytest.approx(0.0)
        assert 0.0 <= result < 2.0 * math.pi
    
    def test_negative_one_full_rotation(self):
        """测试输入为 -2π（反向一圈）"""
        angle = -2.0 * math.pi
        result = normalize_angle(angle)
        assert result == pytest.approx(0.0)
        assert 0.0 <= result < 2.0 * math.pi
    
    def test_negative_two_full_rotations(self):
        """测试输入为 -4π（反向两圈）"""
        angle = -4.0 * math.pi
        result = normalize_angle(angle)
        assert result == pytest.approx(0.0)
        assert 0.0 <= result < 2.0 * math.pi
    
    # ==================== 各种角度组合测试 ====================
    
    def test_combination_quadrant_1(self):
        """测试第一象限角度（0 到 π/2）"""
        test_angles = [0.0, math.pi/6, math.pi/4, math.pi/3, math.pi/2 - 1e-10]
        for angle in test_angles:
            result = normalize_angle(angle)
            assert result == pytest.approx(angle)
            assert 0.0 <= result < 2.0 * math.pi
    
    def test_combination_quadrant_2(self):
        """测试第二象限角度（π/2 到 π）"""
        test_angles = [math.pi/2 + 0.001, 2.0*math.pi/3, 3.0*math.pi/4, 5.0*math.pi/6]
        for angle in test_angles:
            result = normalize_angle(angle)
            assert result == pytest.approx(angle)
            assert 0.0 <= result < 2.0 * math.pi
    
    def test_combination_quadrant_3(self):
        """测试第三象限角度（π 到 3π/2）"""
        test_angles = [math.pi + 0.001, 4.0*math.pi/3, 5.0*math.pi/4, 3.0*math.pi/2 - 0.001]
        for angle in test_angles:
            result = normalize_angle(angle)
            assert result == pytest.approx(angle)
            assert 0.0 <= result < 2.0 * math.pi
    
    def test_combination_quadrant_4(self):
        """测试第四象限角度（3π/2 到 2π）"""
        test_angles = [3.0*math.pi/2 + 0.001, 5.0*math.pi/3, 7.0*math.pi/4, 2.0*math.pi - 0.001]
        for angle in test_angles:
            result = normalize_angle(angle)
            assert result == pytest.approx(angle)
            assert 0.0 <= result < 2.0 * math.pi
    
    # ==================== 边界路径覆盖测试 ====================
    
    def test_path_positive_modulus(self):
        """测试正数取模路径（a >= 0）"""
        angle = 3.0  # 正数，结果在 0~2π 之间
        result = normalize_angle(angle)
        assert result == pytest.approx(angle)
        assert result == 3.0  # 验证走的是 true 分支
    
    def test_path_negative_modulus(self):
        """测试负数取模路径（a < 0，需要加 2π）"""
        angle = -1.0  # 负数
        result = normalize_angle(angle)
        expected = 2.0 * math.pi - 1.0
        assert result == pytest.approx(expected)
        # 验证走的是 false 分支
    
    # ==================== 精度测试 ====================
    
    def test_precision_high(self):
        """测试高精度的角度值"""
        angle = 1.23456789
        result = normalize_angle(angle)
        assert result == pytest.approx(angle, rel=1e-10)
        assert 0.0 <= result < 2.0 * math.pi
    
    def test_precision_negative_high(self):
        """测试高精度负数角度值"""
        angle = -1.23456789
        result = normalize_angle(angle)
        expected = 2.0 * math.pi - 1.23456789
        assert result == pytest.approx(expected, rel=1e-10)
        assert 0.0 <= result < 2.0 * math.pi
    
    def test_return_type_float(self):
        """测试返回类型为 float"""
        angle = math.pi / 3
        result = normalize_angle(angle)
        assert isinstance(result, float)
        assert 0.0 <= result < 2.0 * math.pi