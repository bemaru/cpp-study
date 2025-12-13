# String To Uint64 Using KnuthHash

* 요약

  문자열의 해시(Knuth hash)정수를 구하는 C++, C# 코드

* 배경

  C++과 C#에서 문자열의 동일한 해시정수가 필요했습니다.

  ( 특정 파일경로를 PK로 사용하는데 문자열이 아닌 정수(해시값)를 사용하기 위해서)
  

cpp
<pre><code>
UINT64 CalculateHash(CString read){
	UINT64 hashedValue = 3074457345618258791ull;
	for (int i = 0; i < read.GetLength(); i++)
	{
		hashedValue += read[i];
		hashedValue *= 3074457345618258799ull;
	}
	return hashedValue;
}
</code></pre>

c#
<pre><code>
UInt64 CalculateHash(string read)
{
    UInt64 hashedValue = 3074457345618258791ul;
    for (int i = 0; i < read.Length; i++)
    {
        hashedValue += read[i];
        hashedValue *= 3074457345618258799ul;
    }
    return hashedValue;
}
</code></pre>
