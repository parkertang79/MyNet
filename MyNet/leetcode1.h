#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include<algorithm>
#include <map>
#include <set>
#include <stack>
#include <queue>


//
// Auther: parker tang
// Date :   2024-09-19
// Desc:    本文记录刷leetCode 的遇到的算法
//


#if _MSC_VER > 1020 // if VC++ version is > 4.2
using namespace std; // std c++ libs implemented in std
#endif



//原题网址：https://leetcode.cn/problems/two-sum/?envType=study-plan-v2&envId=top-100-liked
/*
给定一个整数数组 nums 和一个整数目标值 target，请你在该数组中找出 和为目标值 target  的那 两个 整数，并返回它们的数组下标。

你可以假设每种输入只会对应一个答案，并且你不能使用两次相同的元素。

你可以按任意顺序返回答案。



示例 1：

输入：nums = [2,7,11,15], target = 9
输出：[0,1]
解释：因为 nums[0] + nums[1] == 9 ，返回 [0, 1] 。
示例 2：

输入：nums = [3,2,4], target = 6
输出：[1,2]
示例 3：

输入：nums = [3,3], target = 6
输出：[0,1]

*/

 void twoSum(void)
{
	vector<int> nums = { 2,7,11,15 };
	int target = 9;
	vector<int> result;

    //algorithm code
	//1.第一种方法，暴力搜索，时间复杂度 N*N
	/*
	for (int i = 0; i < nums.size(); i++)
	{
		int one = target - nums[i];
		if (one < 1) continue;  // 可减少某些比较次数

		for (int j = i + 1; j < nums.size(); j++)
		{
			if (nums[j] == one)
			{
				result.push_back(i);
				result.push_back(j);
				goto RESULT;
			}
		}
	}
	*/

	//2.使用哈希表，将第二个for 循环消除，时间复杂度 N
	{
		unordered_map<int, int> data;
		for (int i = 0; i < nums.size(); i++)
		{
			data[nums[i]] = i;
		}

		for (int i = 0; i < nums.size(); i++)
		{
			int one = target - nums[i];
			if (one < 1) continue;  // 可减少某些比较次数
			if (data.find(one) != data.end())
			{
				result.push_back(i);
				result.push_back(data[one]);
				goto RESULT;
			}
		}
	}

RESULT:
	if (result.size() > 0)
	{
		printf("\n+++++++++++++++++++++++++++++++++++++++++++++++++\n");
		printf("\ntwo Sum result is : [%d,%d]\n\n", result[0], result[1]);
		printf("\n+++++++++++++++++++++++++++++++++++++++++++++++++\n");
	}

}

 //原题网址：https://leetcode.cn/problems/group-anagrams/solutions/2718519/ha-xi-biao-fen-zu-jian-ji-xie-fa-pythonj-1ukv/?envType=study-plan-v2&envId=top-100-liked
 /*
 给你一个字符串数组，请你将 字母异位词 组合在一起。可以按任意顺序返回结果列表。

字母异位词 是由重新排列源单词的所有字母得到的一个新单词。

 

示例 1:

输入: strs = ["eat", "tea", "tan", "ate", "nat", "bat"]
输出: [["bat"],["nat","tan"],["ate","eat","tea"]]
示例 2:

输入: strs = [""]
输出: [[""]]
示例 3:

输入: strs = ["a"]
输出: [["a"]]
 

提示：

1 <= strs.length <= 104
0 <= strs[i].length <= 100
strs[i] 仅包含小写字母
 */

 void groupAnagrams(void)
 {

	 vector<string> strs = { "eat", "tea", "tan", "ate", "nat", "bat" };
	 vector<vector<string>> result;

	 //方法1：以字符串数组元素的排序后字符串为key，以数组中序列号为 value，构建hash 
	 {
		 unordered_map<string, vector<string>> data;
		 for (int i = 0; i < strs.size(); i++)
		 {
			 string s = strs[i];
			 //先sort 字符串，这样就能利用hash 的特点降低时间复杂度了。
			 sort(s.begin(), s.end());
			 data[s].push_back(strs[i]);
			 /*
			 // 下面是多余的判断，map 中存不存在 key，都可以直接调用 value 的相关函数
			 if (data.find(s) != data.end())
			 {
				 data[s].push_back(strs[i]);
			 }
			 else
			 {
				 data[s] = vector<string>{strs[i]};
			 }
			 */
		 }
		 // 遍历map ，提取最终的结果
		 for (auto it = data.begin(); it != data.end(); it++)
		 {
			 result.push_back(it->second);
		 }
	 }
 
 RESULT:
	 if (result.size() > 0)
	 {
		 printf("\n+++++++++++++++++++++++++++++++++++++++++++++++++\n");
		 printf("\nResult is: \n[");
		 for (auto ita = result.begin(); ita != result.end(); ita++)
		 {
			 printf("[ ");
			 for (auto itb = ita->begin(); itb != ita->end(); itb++)
			 {
				 printf(" \"%s\", ", itb->c_str());
			 }
			 printf(" ],");
		 }
		 printf(" ]\n");
		 printf("\n+++++++++++++++++++++++++++++++++++++++++++++++++\n");
	 }
 }


 //原题网址：https://leetcode.cn/problems/longest-consecutive-sequence/?envType=study-plan-v2&envId=top-100-liked
 /*
 给定一个未排序的整数数组 nums ，找出数字连续的最长序列（不要求序列元素在原数组中连续）的长度。

请你设计并实现时间复杂度为 O(n) 的算法解决此问题。

 
示例 1：

输入：nums = [100,4,200,1,3,2]
输出：4
解释：最长数字连续序列是 [1, 2, 3, 4]。它的长度为 4。
示例 2：

输入：nums = [0,3,7,2,5,8,4,6,0,1]
输出：9
 

提示：

0 <= nums.length <= 105
-109 <= nums[i] <= 109
 */

 void longestConsecutive(void)
 {
	 vector<int> nums = { 0,3,7,2,5,8,4,6,0,1 };
	 int count = 0;

	 //要求时间复杂度是N，排除掉暴力搜索的解法
	 //方法一，map的key 是一个有序的，可利用这点
	 if(0)
	 {
		 map<int, int> data;
		 for (int i = 0; i < nums.size(); i++)
		 {
			 data[nums[i]] = 1;
		 }

		 int len = 0;
		 auto it = data.begin();
		 int old = it->first;
		 for (; it != data.end(); it++)
		 {
			 if (it->first == old + 1)
			 {
				 len++;
			 }
			 else
			 {
				 if (len > count) count = len;
				 len = 1;
			 }
			 old = it->first;
		 }

		 if (len > count) count = len;
	 }

	 //方法二，利用计算右边界来获取最大连接长度 
	 if(0)
	 {
		 unordered_map<int, int> data;
		 for (int i = 0; i < nums.size(); i++)
		 {
			 data[nums[i]] = 1;
		 }

		 //计算右边界长度
		 for (int num:nums)
		 {
			 //如果存在左边界，不再计算它的右边界长度，显然左边的那个数据的右边界比它更大
			 if (data.find(num - 1) != data.end()) continue; 
			 //查询右边界最大值
			 int right = num + 1;
			 for ( ; data.find(right) != data.end(); right++);
			 //设置最大值
			 if (right - num > count)  count = right - num;
		 }
	 }

	 //方法三，遍历并计算每个点的最长连接区间
	 //来源：https://leetcode.cn/problems/longest-consecutive-sequence/solutions/3956/dong-tai-gui-hua-python-ti-jie-by-jalan/
	 if (0)
	 {
		 //定义hash表， key 表示数组中的元素值，value 表示这个元素的最长连接区间，初始值为1（元素本身占据一个位置）
		 unordered_map<int, int >data;

		 for (int num : nums)
		 {
			 // 如果在hash 中存在，则不必处理
			 if (data.find(num) != data.end()) continue;

			 // 每次遇到一个新的数组元素，先查看它的左侧和右侧是否有元素在 哈希表中存在，如果存在，则 最长连接区间 = left + right + 1
			 int left = 0, right = 0;
			 if (data.find(num - 1) != data.end()) left = data[num - 1];
			 if (data.find(num + 1) != data.end()) right = data[num + 1];
			 int length = left + right + 1;

			 //更新hash表中这一连接区域两个端点的数据
			 data[num - left] = length;
			 if (right) data[num + right] = length;

			 //设置最大值
			 if (length > count) count = length;
		 }

	 }

	 //方法四，查并集
	 //来源：https://leetcode.cn/problems/longest-consecutive-sequence/solutions/3956/dong-tai-gui-hua-python-ti-jie-by-jalan/
	 if (1)
	 {
		   //定义hash，key 为数组中元素，value 为其最右边的元素，初始化为其本身
		 unordered_map<int, int> data;
		 for (int num : nums)
		 {
			 data[num] = num;
		 }

		 //与右侧临近元素归并
		 for (int num : nums)
		 {
			 int right = num + 1;
			 int old = num;
			 int ll = 0;

			 // 无右侧元素，忽略
			 if ((data.find(right) == data.end())) continue;

			 //查找最右侧的端点
			 vector<int> nn = {num};
			 while (data.find(right) != data.end())
			 {
				 nn.push_back(right);
				 if (right == data[right])
				 { 
					 right = right + 1;
				 }
				 else
				 {
					 right = data[right];
				 }
			 }

			 //归并
			 old = nn[nn.size() - 1];
			for (int a: nn)
			{
				 if(data[a] != old) data[a] = old;
			}
			//计算当前元素的右侧区间长度
		    ll = old - num + 1;

			 //设置最大连接长度
			 if (count <ll) count = ll;
		 }

	 }
 
 RESULT:

	 {
		 printf("\n+++++++++++++++++++++++++++++++++++++++++++++++++\n");
		 printf("\nResult is:   %d\n",count);
		 printf("\n+++++++++++++++++++++++++++++++++++++++++++++++++\n");
	 }
 }


 //原题:  https://leetcode.cn/problems/move-zeroes/description/?envType=study-plan-v2&envId=top-100-liked
 /*
 给定一个数组 nums，编写一个函数将所有 0 移动到数组的末尾，同时保持非零元素的相对顺序。

请注意 ，必须在不复制数组的情况下原地对数组进行操作。

 

示例 1:

输入: nums = [0,1,0,3,12]
输出: [1,3,12,0,0]
示例 2:

输入: nums = [0]
输出: [0]
 

提示:

1 <= nums.length <= 104
-231 <= nums[i] <= 231 - 1
 

进阶：你能尽量减少完成的操作次数吗？
 */
 void moveZeros(void)
 {
	 vector<int> nums = { 0,1,0,3,12 };

	 //方法，利用两个快慢指针，快指针向前遍历，慢指针指向 0，两者交换元素
	 if (1)
	 {
		 int slow = 0,fast = 1;
		 /*
		 while (fast < nums.size())
		 {
			 if (nums[fast] == 0)
			 {
				 if (nums[slow])
				 { 
					 slow++;
				 }
				 
			 }
			 else
			 {
				 if (nums[slow] == 0)
				 {
					 swap(nums[slow], nums[fast]);
					 slow++;
				 }
			 }
			 fast++;
		 }
		 */
		 // 简化一些判断条件
		 while (fast < nums.size())
		 {
			 if (nums[fast] && nums[slow] == 0)
			 {
					 swap(nums[slow], nums[fast]);
					 slow++;
			 }
			 fast++;
		 }
	 }


 RESULT:
	 {
		 printf("\n+++++++++++++++++++++++++++++++++++++++++++++++++\n");
		 printf("\nResult is:   ");
		 for (int num : nums) printf("  %d  ", num);
		 printf("\n+++++++++++++++++++++++++++++++++++++++++++++++++\n");
	 }
 }

 //原题：https://leetcode.cn/problems/container-with-most-water/description/?envType=study-plan-v2&envId=top-100-liked
 /*
 给定一个长度为 n 的整数数组 height 。有 n 条垂线，第 i 条线的两个端点是 (i, 0) 和 (i, height[i]) 。

找出其中的两条线，使得它们与 x 轴共同构成的容器可以容纳最多的水。

返回容器可以储存的最大水量。

说明：你不能倾斜容器。

 输入：[1,8,6,2,5,4,8,3,7]
输出：49 
解释：图中垂直线代表输入数组 [1,8,6,2,5,4,8,3,7]。在此情况下，容器能够容纳水（表示为蓝色部分）的最大值为 49。
示例 2：

输入：height = [1,1]
输出：1
 

提示：

n == height.length
2 <= n <= 105
0 <= height[i] <= 104

 */
 void maxArea(void)
 {
	 vector<int> heights = { 1,8,6,2,5,4,8,3,7 };
	 int maxarea = 0;

	 //方法一，暴力遍历，计算面积
	 if (0)
	 {
		 int len = heights.size();
		 for (int left = 0; left < len; left++)
		 {
			 //以此为左边界，计算的最大的面积低于maxarea,就不能以此为左边界。
			 int area = heights[left] * (len - left - 1);
			 if (area < maxarea) continue;

			 for (int right = len - 1; right > left; right--)
			 {
				 //以此为右边界，以heights[left] 为高度，计算的面积低于maxarea,就不需要继续向左遍历了。
				 area = heights[left] * (right - left);
				 if (area < maxarea) break;

				 area = min(heights[left], heights[right]) * (right - left);
				 if (area > maxarea) maxarea = area;

			 }
		 }
	 }

	 //方法二， 从左右两个方向往中间遍历
	 if (1)
	 {
		 int left = 0, right = heights.size() -1;
		 while (left < right)
		 {
			 int area = min(heights[left], heights[right]) * (right - left);
			 maxarea = max(maxarea, area);
			 if (heights[left] < heights[right])
			 {
				 left++;
			 }
			 else
			 {
				 right--;
			 }
		 }
	 }
 RESULT:
	 {
		 printf("\n+++++++++++++++++++++++++++++++++++++++++++++++++\n");
		 printf("\nResult is:  %d ",maxarea);
		 printf("\n+++++++++++++++++++++++++++++++++++++++++++++++++\n");
	 }
 }

 //原题：https://leetcode.cn/problems/3sum/description/?envType=study-plan-v2&envId=top-100-liked
 /*
 给你一个整数数组 nums ，判断是否存在三元组 [nums[i], nums[j], nums[k]] 满足 i != j、i != k 且 j != k ，同时还满足 nums[i] + nums[j] + nums[k] == 0 。请你返回所有和为 0 且不重复的三元组。

注意：答案中不可以包含重复的三元组。

 
示例 1：

输入：nums = [-1,0,1,2,-1,-4]
输出：[[-1,-1,2],[-1,0,1]]
解释：
nums[0] + nums[1] + nums[2] = (-1) + 0 + 1 = 0 。
nums[1] + nums[2] + nums[4] = 0 + 1 + (-1) = 0 。
nums[0] + nums[3] + nums[4] = (-1) + 2 + (-1) = 0 。
不同的三元组是 [-1,0,1] 和 [-1,-1,2] 。
注意，输出的顺序和三元组的顺序并不重要。
示例 2：

输入：nums = [0,1,1]
输出：[]
解释：唯一可能的三元组和不为 0 。
示例 3：

输入：nums = [0,0,0]
输出：[[0,0,0]]
解释：唯一可能的三元组和为 0 。
 

提示：

3 <= nums.length <= 3000
-105 <= nums[i] <= 105
 */
 void threeSum(void)
 {
	 vector<int> nums = { -1,0,1,2,-1,-4 };
	 vector<vector<int>> result;

	 //方法1，先sort ，再利用 a+b+c = 0 的特点遍历
	 if (0)
	 {
		 sort(nums.begin(), nums.end());
		 unordered_map<int, int> data; //判断第三个元素
		 for (int n : nums)
		 {
			 data[n] = 1;
		 }

		 int left = 0;
		 int right = nums.size() - 1;
		 int m = 0;
		 while (left + 1 < right && nums[left] <= 0)
		 {
			 m = 0 - nums[left] - nums[right];
			 if (m >= nums[left + 1] && m <= nums[right - 1])
			 {
				 if (data.find(m) != data.end())
				 {
					 vector<int> r = { nums[left],m,nums[right] };
					 result.push_back(r);
					 //printf(" %d   %d   %d  \n", nums[left], m, nums[right]);
				 }
			 }

			 right--;
			 while (right > left + 1 && nums[right] == nums[right + 1]) right--;
			 if (nums[right] < 0 || left + 2 > right)
			 {
				 right = nums.size() - 1;
				 left++;
				 while (left + 1 < right && nums[left] == nums[left - 1]) left++;
			 }
		 }
	 }
		 //方法2，先sort ，再遍历数组
	 if (1)
	 {
		 sort(nums.begin(), nums.end());

		 int nn = nums.size();
		 int left = 0;
		 int right = nn - 1;
		 int m = 0;
		 for (; left < nn - 2;)
		 {
			 //最小值left必然小于等于 0
			 if (nums[left] > 0) break;
			 //以left 开始的连续三个元素之和大于 0，则后面的遍历之和必然大于0
			 if (nums[left] + nums[left + 1] + nums[left + 2] > 0) break;

			 for (right = left + 2; right < nn; )
			 {
				 //三个元素之和大于0，则后面的遍历之和必然大于0
				 if (nums[left] + nums[left + 1] + nums[right] > 0) break;
				 int d = 0 - nums[left] - nums[right];
				 for (m = left + 2; m < right && nums[m] <= d; m++);
				 if (nums[m - 1] == d)
				 {
					 vector<int> r = { nums[left],nums[m - 1],nums[right] };
					 result.push_back(r);
				 }

				 //去除连续的元素
				 right++;
				 while (right < nn  && nums[right] == nums[right - 1]) right++;
			 }

			 //去除连续的元素
			 left++;
			 while (left < nn - 2 && nums[left] == nums[left - 1]) left++;
		 }
	 }
	

 RESULT:
	 {
		 printf("\n+++++++++++++++++++++++++++++++++++++++++++++++++\n");
		 printf("\nResult is:  [ ");
		 for (auto a : result)
		 {
			 printf(" [ ");
			 for (auto b : a)
			 { 
				 printf(" %d ", b);
			 }
			 printf("  ], ");
		 }
		 printf(" ]\n+++++++++++++++++++++++++++++++++++++++++++++++++\n");
	 }
 }

 //原题：https://leetcode.cn/problems/trapping-rain-water/description/?envType=study-plan-v2&envId=top-100-liked
 /*
 给定 n 个非负整数表示每个宽度为 1 的柱子的高度图，计算按此排列的柱子，下雨之后能接多少雨水。
 

示例 1：



输入：height = [0,1,0,2,1,0,1,3,2,1,2,1]
输出：6
解释：上面是由数组 [0,1,0,2,1,0,1,3,2,1,2,1] 表示的高度图，在这种情况下，可以接 6 个单位的雨水（蓝色部分表示雨水）。 
示例 2：

输入：height = [4,2,0,3,2,5]
输出：9
 

提示：

n == height.length
1 <= n <= 2 * 104
0 <= height[i] <= 105
 
 */

 int getmax(vector<int>& h, int ll, int rr)
 {
	 printf("\n getmax left:  %d   right: %d \n", ll, rr);
	 if (rr < ll + 1) return 0;

	 int sum = 0;
	 //get left
	 int lm = ll + 1;
	 while (lm <= rr && h[lm] >= h[lm - 1]) lm++;
	 lm--;
	 if (rr < lm+1) return 0;
	 

	 //get right
	 int rm = lm + 1;
	 int rm2 = rm+1;
	 int s = 0;
	 int s1 = 0;
	 int s2 = 0;
	 for (int n = lm + 1; n <= rr; n++)
	 {
		 if (h[n] >= h[lm])
		 {
			  rm = n;
			  rm2 = n;
			  sum += s;
			  printf("\n getmax left:  %d   right: %d  sum :%d\n", ll, n,sum);
			  return sum + getmax(h,n,rr);
		 }

		 if (h[n] >= h[rm])
		 {
			 rm = n;
			 rm2 = n + 1;
			 s1 = s;
		 }
		 else if (h[n] >= h[rm2])
		 {
			 rm2 = n;
			 s2 = s - s1;
		 }

		 s += h[lm] - h[n];
	 }

	 if (rm > lm + 1)
	 {
		 sum += s1 + (h[rm] - h[lm]) * (rm - lm - 1);
		 if (rm2 > rm + 1)
		 {
			 sum += s2 +h[rm] - h[lm] + (h[rm2] - h[lm]) * (rm2 - rm - 1);
			 printf("\n 0getmax left:  %d   right: %d  sum :%d\n", lm, rm2, sum);
			 return sum + getmax(h, rm2, rr);
		 }
		 else
		 {
			 printf("\n 1getmax left:  %d   right: %d  sum :%d\n", lm, rm, sum);
			 return sum + getmax(h, rm, rr);
		 }
	 }
	 else
	 {
		 if (rm2 > rm + 1)
		 {
			 sum += s2 +h[rm] - h[lm]+(h[rm2] - h[lm]) * (rm2 - rm-1);
			 printf("\n 2getmax left:  %d   right: %d  sum :%d\n", lm, rm2, sum);
			 return sum + getmax(h, rm2, rr);
		 }
		 else
		 {
			 printf("\n 3getmax left:  %d   right: %d  sum :%d\n", lm, rm, sum);
			 return sum + getmax(h, rm, rr);
		 }
	 }

	 return sum ;
 }


 void trap(void)
 {
	 vector<int> height = { 4,3,1,2 };
	 int sum = 0;

	 //方法一：从左边开始一个个寻找能接到雨水的容器，一个个单独计算
	 if (0)
	 {
		 int ll = 0;
		 int rr = height.size() - 1;

		 sum = getmax(height,  ll, rr);
 		
	 }

	 //方法二：计算每个元素的左边最大值，和右边最大值，然后根据左右边界极值计算每个元素位置的雨水
	 //来源：https://www.bookstack.cn/read/fucking-algorithm/%E9%AB%98%E9%A2%91%E9%9D%A2%E8%AF%95%E7%B3%BB%E5%88%97-%E6%8E%A5%E9%9B%A8%E6%B0%B4.md
	 if (0)
	 {
		 unordered_map<int, int> lmax;
		 unordered_map<int, int> rmax;
		 int hh = height.size();
		 lmax[0] = height[0];
		 for (int n = 1; n < hh; n++)
		 {
			 lmax[n] = max(height[n], lmax[n - 1]);
		 }

		 rmax[hh - 1] = height[hh - 1];
		 for (int n = hh - 2; n >= 0; n--)
		 {
			 rmax[n] = max(height[n], rmax[n + 1]);
		 }

		 for (int n = 0; n < hh; n++)
		 {
			 int mm = min(lmax[n], rmax[n]);
			 if (mm > height[n])
			 {
				 sum += mm - height[n];
			 }
		 }
	 }

	 //方法三： 双指针
	 //来源：https://mp.weixin.qq.com/s/f9ebzbwymR8jQeUDxjeCDA
	 if (0)
	 {
		 int leftmax = 0, rightmax = 0;
		 int left = 0, right = height.size() - 1;
		 while (left <= right)
		 {
			 if (leftmax <= rightmax)
			 {
				 leftmax = max(leftmax, height[left]);
				 sum += leftmax - height[left++];
			 }
			 else
			 {
				 rightmax = max(rightmax, height[right]);
				 sum += rightmax - height[right--];
			 }
		 }
	 }

	 //方法四： 单调栈
	 //来源：https://mp.weixin.qq.com/s/f9ebzbwymR8jQeUDxjeCDA
	 if (1)
	 {
		 stack<int> data;
		 for (int n = 0; n < height.size(); n++)
		 {
			 while (!data.empty() && height[data.top()] < height[n])
			 {
				 int t = data.top();
				 data.pop();

				 while (!data.empty() && height[data.top()] == height[t]) data.pop();

				 if (!data.empty())
				 {
					 int depth = min(height[data.top()], height[n]) - height[t];
					 sum += depth * (n - data.top() - 1);
				 }
			 }

			 data.push(n);
		 }
	 }
 
RESULT:
 {
	 printf("\n+++++++++++++++++++++++++++++++++++++++++++++++++\n");
	 printf("\nResult is:  %d \n",sum);
	 printf(" ]\n+++++++++++++++++++++++++++++++++++++++++++++++++\n");
 }
 }

 //来源：https://leetcode.cn/problems/longest-substring-without-repeating-characters/?envType=study-plan-v2&envId=top-100-liked
 /*
 给定一个字符串 s ，请你找出其中不含有重复字符的最长子串的长度。

 

示例 1:

输入: s = "abcabcbb"
输出: 3 
解释: 因为无重复字符的最长子串是 "abc"，所以其长度为 3。
示例 2:

输入: s = "bbbbb"
输出: 1
解释: 因为无重复字符的最长子串是 "b"，所以其长度为 1。
示例 3:

输入: s = "pwwkew"
输出: 3
解释: 因为无重复字符的最长子串是 "wke"，所以其长度为 3。
     请注意，你的答案必须是 子串 的长度，"pwke" 是一个子序列，不是子串。
 

提示：

0 <= s.length <= 5 * 104
s 由英文字母、数字、符号和空格组成
 
 */
 void lengthOfLongestSubstring(void)
 {
	 std::string s = " ";

	 int maxlen = 0;

	 //方法一，滑动窗口
	 if (1)
	 {
		 int left = 0;
		 int right = 0;
		 int len = s.length();
		 int c = 0;


		 while (++right < len)
		 {
			 for (int n = right - 1; n >= left; n--)
			 {
				 if (s[n] == s[right])
				 {
					 left = n + 1;
					 break;
				 }
			 }

			 c = right - left + 1;
			 maxlen = max(maxlen, c);
		 }

		 if (len < 2)maxlen = len;


	 }
 
 RESULT:
	 {
		 printf("\n+++++++++++++++++++++++++++++++++++++++++++++++++\n");
		 printf("\nResult is:  %d \n", maxlen);
		 printf(" ]\n+++++++++++++++++++++++++++++++++++++++++++++++++\n");
	 }
 }
