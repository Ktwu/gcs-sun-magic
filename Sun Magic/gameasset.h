#pragma once

#include "stdafx.h"

namespace sun_magic {

	template <class T>
	class GameAsset {
	public:
		GameAsset(T* resource = NULL);
		~GameAsset();

		int GetRefCount();
		bool HasGameAsset();
		T* GetRef();
		void ReturnRef();
		T* TrySet(T* resource);


	private:
		T* _resource_;
		int _ref_count_;
	};


	template <class T>
	GameAsset<T>::GameAsset(T* resource = NULL) {
		_resource_ = resource;
		_ref_count_ = 0;
	}

	template <class T>
	GameAsset<T>::~GameAsset() {
		if (_resource_ != NULL) {
			delete _resource_;
			_resource_ = NULL;
		}
	}

	template <class T>
	int GameAsset<T>::GetRefCount() {
		return _ref_count_;
	}

	template <class T>
	bool GameAsset<T>::HasGameAsset() {
		return _resource_ != NULL;
	}

	template <class T>
	T* GameAsset<T>::GetRef() {
		if (HasGameAsset())
			++_ref_count_;
		return _resource_;
	}

	template <class T>
	void GameAsset<T>::ReturnRef() {
		if (_ref_count_ > 0)
			--_ref_count_;
	}

	template <class T>
	T* GameAsset<T>::TrySet(T* resource) {
		T* old_resource = NULL;
		if (_ref_count_ == 0) {
			old_resource = _resource_;
			_resource_ = resource;
		}
		return old_resource;
	}

}