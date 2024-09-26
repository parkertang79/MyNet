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
// Desc:    ���ļ�¼ˢleetCode ���������㷨
//


#if _MSC_VER > 1020 // if VC++ version is > 4.2
using namespace std; // std c++ libs implemented in std
#endif



//ԭ����ַ��https://leetcode.cn/problems/two-sum/?envType=study-plan-v2&envId=top-100-liked
/*
����һ���������� nums ��һ������Ŀ��ֵ target�������ڸ��������ҳ� ��ΪĿ��ֵ target  ���� ���� ���������������ǵ������±ꡣ

����Լ���ÿ������ֻ���Ӧһ���𰸣������㲻��ʹ��������ͬ��Ԫ�ء�

����԰�����˳�򷵻ش𰸡�



ʾ�� 1��

���룺nums = [2,7,11,15], target = 9
�����[0,1]
���ͣ���Ϊ nums[0] + nums[1] == 9 ������ [0, 1] ��
ʾ�� 2��

���룺nums = [3,2,4], target = 6
�����[1,2]
ʾ�� 3��

���룺nums = [3,3], target = 6
�����[0,1]

*/

 void twoSum(void)
{
	vector<int> nums = { 2,7,11,15 };
	int target = 9;
	vector<int> result;

    //algorithm code
	//1.��һ�ַ���������������ʱ�临�Ӷ� N*N
	/*
	for (int i = 0; i < nums.size(); i++)
	{
		int one = target - nums[i];
		if (one < 1) continue;  // �ɼ���ĳЩ�Ƚϴ���

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

	//2.ʹ�ù�ϣ�����ڶ���for ѭ��������ʱ�临�Ӷ� N
	{
		unordered_map<int, int> data;
		for (int i = 0; i < nums.size(); i++)
		{
			data[nums[i]] = i;
		}

		for (int i = 0; i < nums.size(); i++)
		{
			int one = target - nums[i];
			if (one < 1) continue;  // �ɼ���ĳЩ�Ƚϴ���
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

 //ԭ����ַ��https://leetcode.cn/problems/group-anagrams/solutions/2718519/ha-xi-biao-fen-zu-jian-ji-xie-fa-pythonj-1ukv/?envType=study-plan-v2&envId=top-100-liked
 /*
 ����һ���ַ������飬���㽫 ��ĸ��λ�� �����һ�𡣿��԰�����˳�򷵻ؽ���б�

��ĸ��λ�� ������������Դ���ʵ�������ĸ�õ���һ���µ��ʡ�

 

ʾ�� 1:

����: strs = ["eat", "tea", "tan", "ate", "nat", "bat"]
���: [["bat"],["nat","tan"],["ate","eat","tea"]]
ʾ�� 2:

����: strs = [""]
���: [[""]]
ʾ�� 3:

����: strs = ["a"]
���: [["a"]]
 

��ʾ��

1 <= strs.length <= 104
0 <= strs[i].length <= 100
strs[i] ������Сд��ĸ
 */

 void groupAnagrams(void)
 {

	 vector<string> strs = { "eat", "tea", "tan", "ate", "nat", "bat" };
	 vector<vector<string>> result;

	 //����1�����ַ�������Ԫ�ص�������ַ���Ϊkey�������������к�Ϊ value������hash 
	 {
		 unordered_map<string, vector<string>> data;
		 for (int i = 0; i < strs.size(); i++)
		 {
			 string s = strs[i];
			 //��sort �ַ�����������������hash ���ص㽵��ʱ�临�Ӷ��ˡ�
			 sort(s.begin(), s.end());
			 data[s].push_back(strs[i]);
			 /*
			 // �����Ƕ�����жϣ�map �д治���� key��������ֱ�ӵ��� value ����غ���
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
		 // ����map ����ȡ���յĽ��
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


 //ԭ����ַ��https://leetcode.cn/problems/longest-consecutive-sequence/?envType=study-plan-v2&envId=top-100-liked
 /*
 ����һ��δ������������� nums ���ҳ���������������У���Ҫ������Ԫ����ԭ�������������ĳ��ȡ�

������Ʋ�ʵ��ʱ�临�Ӷ�Ϊ O(n) ���㷨��������⡣

 
ʾ�� 1��

���룺nums = [100,4,200,1,3,2]
�����4
���ͣ���������������� [1, 2, 3, 4]�����ĳ���Ϊ 4��
ʾ�� 2��

���룺nums = [0,3,7,2,5,8,4,6,0,1]
�����9
 

��ʾ��

0 <= nums.length <= 105
-109 <= nums[i] <= 109
 */

 void longestConsecutive(void)
 {
	 vector<int> nums = { 0,3,7,2,5,8,4,6,0,1 };
	 int count = 0;

	 //Ҫ��ʱ�临�Ӷ���N���ų������������Ľⷨ
	 //����һ��map��key ��һ������ģ����������
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

	 //�����������ü����ұ߽�����ȡ������ӳ��� 
	 if(0)
	 {
		 unordered_map<int, int> data;
		 for (int i = 0; i < nums.size(); i++)
		 {
			 data[nums[i]] = 1;
		 }

		 //�����ұ߽糤��
		 for (int num:nums)
		 {
			 //���������߽磬���ټ��������ұ߽糤�ȣ���Ȼ��ߵ��Ǹ����ݵ��ұ߽��������
			 if (data.find(num - 1) != data.end()) continue; 
			 //��ѯ�ұ߽����ֵ
			 int right = num + 1;
			 for ( ; data.find(right) != data.end(); right++);
			 //�������ֵ
			 if (right - num > count)  count = right - num;
		 }
	 }

	 //������������������ÿ��������������
	 //��Դ��https://leetcode.cn/problems/longest-consecutive-sequence/solutions/3956/dong-tai-gui-hua-python-ti-jie-by-jalan/
	 if (0)
	 {
		 //����hash�� key ��ʾ�����е�Ԫ��ֵ��value ��ʾ���Ԫ�ص���������䣬��ʼֵΪ1��Ԫ�ر���ռ��һ��λ�ã�
		 unordered_map<int, int >data;

		 for (int num : nums)
		 {
			 // �����hash �д��ڣ��򲻱ش���
			 if (data.find(num) != data.end()) continue;

			 // ÿ������һ���µ�����Ԫ�أ��Ȳ鿴���������Ҳ��Ƿ���Ԫ���� ��ϣ���д��ڣ�������ڣ��� ��������� = left + right + 1
			 int left = 0, right = 0;
			 if (data.find(num - 1) != data.end()) left = data[num - 1];
			 if (data.find(num + 1) != data.end()) right = data[num + 1];
			 int length = left + right + 1;

			 //����hash������һ�������������˵������
			 data[num - left] = length;
			 if (right) data[num + right] = length;

			 //�������ֵ
			 if (length > count) count = length;
		 }

	 }

	 //�����ģ��鲢��
	 //��Դ��https://leetcode.cn/problems/longest-consecutive-sequence/solutions/3956/dong-tai-gui-hua-python-ti-jie-by-jalan/
	 if (1)
	 {
		   //����hash��key Ϊ������Ԫ�أ�value Ϊ�����ұߵ�Ԫ�أ���ʼ��Ϊ�䱾��
		 unordered_map<int, int> data;
		 for (int num : nums)
		 {
			 data[num] = num;
		 }

		 //���Ҳ��ٽ�Ԫ�ع鲢
		 for (int num : nums)
		 {
			 int right = num + 1;
			 int old = num;
			 int ll = 0;

			 // ���Ҳ�Ԫ�أ�����
			 if ((data.find(right) == data.end())) continue;

			 //�������Ҳ�Ķ˵�
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

			 //�鲢
			 old = nn[nn.size() - 1];
			for (int a: nn)
			{
				 if(data[a] != old) data[a] = old;
			}
			//���㵱ǰԪ�ص��Ҳ����䳤��
		    ll = old - num + 1;

			 //����������ӳ���
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


 //ԭ��:  https://leetcode.cn/problems/move-zeroes/description/?envType=study-plan-v2&envId=top-100-liked
 /*
 ����һ������ nums����дһ������������ 0 �ƶ��������ĩβ��ͬʱ���ַ���Ԫ�ص����˳��

��ע�� �������ڲ���������������ԭ�ض�������в�����

 

ʾ�� 1:

����: nums = [0,1,0,3,12]
���: [1,3,12,0,0]
ʾ�� 2:

����: nums = [0]
���: [0]
 

��ʾ:

1 <= nums.length <= 104
-231 <= nums[i] <= 231 - 1
 

���ף����ܾ���������ɵĲ���������
 */
 void moveZeros(void)
 {
	 vector<int> nums = { 0,1,0,3,12 };

	 //������������������ָ�룬��ָ����ǰ��������ָ��ָ�� 0�����߽���Ԫ��
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
		 // ��һЩ�ж�����
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

 //ԭ�⣺https://leetcode.cn/problems/container-with-most-water/description/?envType=study-plan-v2&envId=top-100-liked
 /*
 ����һ������Ϊ n ���������� height ���� n �����ߣ��� i ���ߵ������˵��� (i, 0) �� (i, height[i]) ��

�ҳ����е������ߣ�ʹ�������� x �Ṳͬ���ɵ�����������������ˮ��

�����������Դ�������ˮ����

˵�����㲻����б������

 ���룺[1,8,6,2,5,4,8,3,7]
�����49 
���ͣ�ͼ�д�ֱ�ߴ����������� [1,8,6,2,5,4,8,3,7]���ڴ�����£������ܹ�����ˮ����ʾΪ��ɫ���֣������ֵΪ 49��
ʾ�� 2��

���룺height = [1,1]
�����1
 

��ʾ��

n == height.length
2 <= n <= 105
0 <= height[i] <= 104

 */
 void maxArea(void)
 {
	 vector<int> heights = { 1,8,6,2,5,4,8,3,7 };
	 int maxarea = 0;

	 //����һ�������������������
	 if (0)
	 {
		 int len = heights.size();
		 for (int left = 0; left < len; left++)
		 {
			 //�Դ�Ϊ��߽磬����������������maxarea,�Ͳ����Դ�Ϊ��߽硣
			 int area = heights[left] * (len - left - 1);
			 if (area < maxarea) continue;

			 for (int right = len - 1; right > left; right--)
			 {
				 //�Դ�Ϊ�ұ߽磬��heights[left] Ϊ�߶ȣ�������������maxarea,�Ͳ���Ҫ������������ˡ�
				 area = heights[left] * (right - left);
				 if (area < maxarea) break;

				 area = min(heights[left], heights[right]) * (right - left);
				 if (area > maxarea) maxarea = area;

			 }
		 }
	 }

	 //�������� �����������������м����
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

 //ԭ�⣺https://leetcode.cn/problems/3sum/description/?envType=study-plan-v2&envId=top-100-liked
 /*
 ����һ���������� nums ���ж��Ƿ������Ԫ�� [nums[i], nums[j], nums[k]] ���� i != j��i != k �� j != k ��ͬʱ������ nums[i] + nums[j] + nums[k] == 0 �����㷵�����к�Ϊ 0 �Ҳ��ظ�����Ԫ�顣

ע�⣺���в����԰����ظ�����Ԫ�顣

 
ʾ�� 1��

���룺nums = [-1,0,1,2,-1,-4]
�����[[-1,-1,2],[-1,0,1]]
���ͣ�
nums[0] + nums[1] + nums[2] = (-1) + 0 + 1 = 0 ��
nums[1] + nums[2] + nums[4] = 0 + 1 + (-1) = 0 ��
nums[0] + nums[3] + nums[4] = (-1) + 2 + (-1) = 0 ��
��ͬ����Ԫ���� [-1,0,1] �� [-1,-1,2] ��
ע�⣬�����˳�����Ԫ���˳�򲢲���Ҫ��
ʾ�� 2��

���룺nums = [0,1,1]
�����[]
���ͣ�Ψһ���ܵ���Ԫ��Ͳ�Ϊ 0 ��
ʾ�� 3��

���룺nums = [0,0,0]
�����[[0,0,0]]
���ͣ�Ψһ���ܵ���Ԫ���Ϊ 0 ��
 

��ʾ��

3 <= nums.length <= 3000
-105 <= nums[i] <= 105
 */
 void threeSum(void)
 {
	 vector<int> nums = { -1,0,1,2,-1,-4 };
	 vector<vector<int>> result;

	 //����1����sort �������� a+b+c = 0 ���ص����
	 if (0)
	 {
		 sort(nums.begin(), nums.end());
		 unordered_map<int, int> data; //�жϵ�����Ԫ��
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
		 //����2����sort ���ٱ�������
	 if (1)
	 {
		 sort(nums.begin(), nums.end());

		 int nn = nums.size();
		 int left = 0;
		 int right = nn - 1;
		 int m = 0;
		 for (; left < nn - 2;)
		 {
			 //��Сֵleft��ȻС�ڵ��� 0
			 if (nums[left] > 0) break;
			 //��left ��ʼ����������Ԫ��֮�ʹ��� 0�������ı���֮�ͱ�Ȼ����0
			 if (nums[left] + nums[left + 1] + nums[left + 2] > 0) break;

			 for (right = left + 2; right < nn; )
			 {
				 //����Ԫ��֮�ʹ���0�������ı���֮�ͱ�Ȼ����0
				 if (nums[left] + nums[left + 1] + nums[right] > 0) break;
				 int d = 0 - nums[left] - nums[right];
				 for (m = left + 2; m < right && nums[m] <= d; m++);
				 if (nums[m - 1] == d)
				 {
					 vector<int> r = { nums[left],nums[m - 1],nums[right] };
					 result.push_back(r);
				 }

				 //ȥ��������Ԫ��
				 right++;
				 while (right < nn  && nums[right] == nums[right - 1]) right++;
			 }

			 //ȥ��������Ԫ��
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

 //ԭ�⣺https://leetcode.cn/problems/trapping-rain-water/description/?envType=study-plan-v2&envId=top-100-liked
 /*
 ���� n ���Ǹ�������ʾÿ�����Ϊ 1 �����ӵĸ߶�ͼ�����㰴�����е����ӣ�����֮���ܽӶ�����ˮ��
 

ʾ�� 1��



���룺height = [0,1,0,2,1,0,1,3,2,1,2,1]
�����6
���ͣ������������� [0,1,0,2,1,0,1,3,2,1,2,1] ��ʾ�ĸ߶�ͼ������������£����Խ� 6 ����λ����ˮ����ɫ���ֱ�ʾ��ˮ���� 
ʾ�� 2��

���룺height = [4,2,0,3,2,5]
�����9
 

��ʾ��

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

	 //����һ������߿�ʼһ����Ѱ���ܽӵ���ˮ��������һ������������
	 if (0)
	 {
		 int ll = 0;
		 int rr = height.size() - 1;

		 sum = getmax(height,  ll, rr);
 		
	 }

	 //������������ÿ��Ԫ�ص�������ֵ�����ұ����ֵ��Ȼ��������ұ߽缫ֵ����ÿ��Ԫ��λ�õ���ˮ
	 //��Դ��https://www.bookstack.cn/read/fucking-algorithm/%E9%AB%98%E9%A2%91%E9%9D%A2%E8%AF%95%E7%B3%BB%E5%88%97-%E6%8E%A5%E9%9B%A8%E6%B0%B4.md
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

	 //�������� ˫ָ��
	 //��Դ��https://mp.weixin.qq.com/s/f9ebzbwymR8jQeUDxjeCDA
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

	 //�����ģ� ����ջ
	 //��Դ��https://mp.weixin.qq.com/s/f9ebzbwymR8jQeUDxjeCDA
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

 //��Դ��https://leetcode.cn/problems/longest-substring-without-repeating-characters/?envType=study-plan-v2&envId=top-100-liked
 /*
 ����һ���ַ��� s �������ҳ����в������ظ��ַ�����Ӵ��ĳ��ȡ�

 

ʾ�� 1:

����: s = "abcabcbb"
���: 3 
����: ��Ϊ���ظ��ַ�����Ӵ��� "abc"�������䳤��Ϊ 3��
ʾ�� 2:

����: s = "bbbbb"
���: 1
����: ��Ϊ���ظ��ַ�����Ӵ��� "b"�������䳤��Ϊ 1��
ʾ�� 3:

����: s = "pwwkew"
���: 3
����: ��Ϊ���ظ��ַ�����Ӵ��� "wke"�������䳤��Ϊ 3��
     ��ע�⣬��Ĵ𰸱����� �Ӵ� �ĳ��ȣ�"pwke" ��һ�������У������Ӵ���
 

��ʾ��

0 <= s.length <= 5 * 104
s ��Ӣ����ĸ�����֡����źͿո����
 
 */
 void lengthOfLongestSubstring(void)
 {
	 std::string s = " ";

	 int maxlen = 0;

	 //����һ����������
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
