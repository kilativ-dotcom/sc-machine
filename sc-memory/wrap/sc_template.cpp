/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc_template.hpp"
#include "sc_memory.hpp"


class ScTemplateGenerator
{
public:
	ScTemplateGenerator(ScTemplate::tReplacementsMap const & replacements,
						ScTemplate::tTemplateConts3Vector const & constructions,
						ScMemoryContext & context)
		: mReplacements(replacements)
		, mConstructions(constructions)
		, mContext(context)
	{
	}

	bool operator() (ScTemplateGenResult & result)
	{
		result.mResult.resize(mConstructions.size() * 3);
		result.mReplacements = mReplacements;

		tAddrList createdElements;
		size_t resultIdx = 0;
		bool isError = false;

		for (ScTemplate::tTemplateConts3Vector::const_iterator it = mConstructions.begin(); it != mConstructions.end(); ++it)
		{
			ScTemplateConstr3 const & item = *it;
			ScTemplateItemValue const * values = item.getValues();

			// check that the third argument isn't a command to generate edge
			check_expr(!(values[2].mItemType == ScTemplateItemValue::VT_Type && values[2].mTypeValue.isEdge()));
			// check that second command is to generate edge
			check_expr(values[1].mItemType == ScTemplateItemValue::VT_Type && values[1].mTypeValue.isEdge());
			// the second item couldn't be a replacement
			check_expr(values[1].mItemType != ScTemplateItemValue::VT_Replace);

			ScAddr const addr1 = resolveAddr(values[0], result.mResult);
			ScAddr const addr2 = resolveAddr(values[2], result.mResult);

			if (!addr1.isValid() || !addr2.isValid())
			{
				isError = true;
				break;
			}

			ScAddr const edge = mContext.createArc(*values[1].mTypeValue, addr1, addr2);
			if (!edge.isValid())
			{
				isError = true;
				break;
			}

			result.mResult[resultIdx++] = addr1;
			result.mResult[resultIdx++] = edge;
			result.mResult[resultIdx++] = addr2;
		}

		if (isError)
		{
			cleanupCreated();
			return false;
		}

		return true;
	}

	ScAddr createNodeLink(ScType const & type)
	{
		ScAddr addr;
		if (type.isNode())
		{
			addr = mContext.createNode(*type);
		}
		else if (type.isLink())
		{
			addr = mContext.createLink();
		}

		if (addr.isValid())
			mCreatedElements.push_back(addr);

		return addr;
	}

	ScAddr resolveAddr(ScTemplateItemValue const & itemValue, tAddrVector const & resultAddrs)
	{
		switch (itemValue.mItemType)
		{
		case ScTemplateItemValue::VT_Addr:
			return itemValue.mAddrValue;
		case ScTemplateItemValue::VT_Type:
			return createNodeLink(itemValue.mTypeValue);
		case ScTemplateItemValue::VT_Replace:
		{
			ScTemplate::tReplacementsMap::const_iterator it = mReplacements.find(itemValue.mReplacementName);
			if (it != mReplacements.end())
			{
				check_expr(it->second < resultAddrs.size());
				return resultAddrs[it->second];
			}
		}
		default:
			break;
		}

		return ScAddr();
	}

	void cleanupCreated()
	{
		for (tAddrList::iterator it = mCreatedElements.begin(); it != mCreatedElements.end(); ++it)
			mContext.eraseElement(*it);
		mCreatedElements.clear();
	}

private:
	ScTemplate::tReplacementsMap const & mReplacements;
	ScTemplate::tTemplateConts3Vector const & mConstructions;
	ScMemoryContext & mContext;
	tAddrList mCreatedElements;
};



bool ScTemplate::generate(ScMemoryContext & ctx,  ScTemplateGenResult & result) const
{
	ScTemplateGenerator gen(mReplacements, mConstructions, ctx);
	return gen(result);
}

