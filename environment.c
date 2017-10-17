/*
 * environment.c
 *
 * Project:      Telvin Interpreter (University of Media)
 * Version:      0.0.1
 * Last edit:    17.10.2017
 */



#include "headers/environment.h"



TEL_OBJ
alloc_environment(unsigned int size)
{
	unsigned int count_bytes;
	TEL_OBJ new_env;
	struct tableEntry *entries;

	count_bytes = size * (unsigned int)(sizeof(struct tableEntry));
	entries = (struct tableEntry *)malloc(count_bytes);
	memset(entries, 0, count_bytes);

	new_env = (TEL_OBJ) malloc(sizeof(struct tel_environment));

	new_env->tel_environment.tag = T_ENVIRONMENT;
	new_env->tel_environment.entries = entries;
	new_env->tel_environment.numSlotsInUse = 0;
	new_env->tel_environment.numSlotsAllocated = size;
	new_env->tel_environment.parent = NULL;
	return new_env;
}

static INLINE unsigned long
H(TEL_OBJ aSchemeObject)
{
	// address of schemeObject IS the hashKey!!!
	// only as long as objects do not move...
	return (unsigned long)aSchemeObject;
}

/*
 * join new method
 */
void supply_environment(TEL_OBJ environment)
{
	unsigned int i, size, new_size;
	struct tableEntry *entries, *new_entries;

	entries = environment->tel_environment.entries;

	size = environment->tel_environment.numSlotsAllocated;
	new_size = size * 2 + 1;

	new_entries = (struct tableEntry *)malloc(new_size * sizeof(struct tableEntry));
	memset(new_entries, 0, new_size * sizeof(struct tableEntry));

	for (i = 0; i < size; i++)
	{
		TEL_OBJ key;

		key = entries[i].key;
		if (key != NULL)
		{
			TEL_OBJ value;
			int j, k;

			value = entries[i].value;

			j = k = (int)( H(key) % new_size );
			while(1)
			{
				if (new_entries[k].key == NULL)
				{
					new_entries[k].key = key;
					new_entries[k].value = value;
					break;
				}
				k = (k + 1) % new_size;
				if (k == j)
				{
					ERROR("cannot happen");
				}
			}
		}
	}
	environment->tel_environment.entries = new_entries;
	environment->tel_environment.numSlotsAllocated = new_size;
}

void
init_environment()
{
	topEnvironment = alloc_environment(INIT_ENVIRONMENT_SIZE);
}

void
add_binding(TEL_OBJ env, TEL_OBJ key, TEL_OBJ value)
{
	int i, j;
	struct tableEntry *join_ptr;

	i = j = (int)( H(key) % env->tel_environment.numSlotsAllocated);
	while (C_TRUE)
	{
		join_ptr = &(env->tel_environment.entries[j]);
		if (join_ptr->key == NULL)
		{
			join_ptr->key = key;
			join_ptr->value = value;
			env->tel_environment.numSlotsInUse++;
			if (env->tel_environment.numSlotsInUse > (env->tel_environment.numSlotsAllocated * 3 / 4))
			{
				supply_environment(env);
			}
			return;
		}
		if (join_ptr->key == key)
		{
			join_ptr->value = value;
			return;
		}

		j = (j + 1) % env->tel_environment.numSlotsAllocated;
		if (j == i)
		{
			ERROR("cannot happen");
		}
	}

	ERROR("unimpl.");
}

TEL_OBJ
get_binding(TEL_OBJ env, TEL_OBJ search_tag)
{
	int i, j;
	struct tableEntry *my_entry;

	CHECK(isSymbol(search_tag), "non-symbol key");

	while (C_TRUE)
	{
		// fprintf(stdout, "the searchedKey is %s {%p}\n", symbolValue(searchedKey), searchedKey);

		i = j = (int)( H(search_tag) % env->tel_environment.numSlotsAllocated );
		while (C_TRUE)
		{
			my_entry = &(env->tel_environment.entries[j]);
			if (my_entry->key == search_tag)
			{
				return (my_entry->value);
			}
			if (my_entry->key == NULL)
			{
				if (env->tel_environment.parent == NULL)
				{
					return NULL;
				}
				env = env->tel_environment.parent;
				break;
			}
			j = (j + 1) % env->tel_environment.numSlotsAllocated;
			if (j == i)
			{
				ERROR("cannot happen");
			}
		}
	}
	return NULL;
}
