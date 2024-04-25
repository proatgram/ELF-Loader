#include <stddef.h>

template <class T>
class linked_list {
    public:
        inline linked_list() :
            m_first(nullptr),
            m_last(nullptr),
            m_size(0)
        {

        }

        ~linked_list() {
            linked_element *element = m_last;

            while (element != nullptr) {
                linked_element *new_element = element->prev;
                delete element;
                element = new_element;
            }

            m_first = nullptr;
            m_last = nullptr;
        }

        int add(T data) {
            linked_element *element = new linked_element{data};
            
            if (m_size == 0) {
                m_first = element;
                m_first->index = 0;
                m_first->prev = nullptr;
                m_first->next = nullptr;

                // In a container of size 1,
                // the last element will
                // always be the first
                m_last = m_first;
                
                return 0;
            }

            element->index = m_last->index + 1;
            element->prev = m_last;
            m_last->next = element;
            m_last = element;

            m_size++;

            return 0;
        }

        int insert(size_t index, T data) {
            if (m_first == nullptr) {
                return -1;
            }

            linked_element *prev_element = nullptr;
            linked_element *element = m_first;

            for (int i = 0; i < index; i++) {
                if (element == nullptr || element->prev == nullptr) {
                    return -1;
                }

                element = element->next;
                prev_element = element->prev;
            }

            linked_element *new_element = new linked_element{data, element, prev_element, prev_element->index + 1};

            prev_element->next = new_element;

            element->prev = new_element;

            update_index(element, 1);
            check_last();

            m_size++;

            return 0;
        }

        int remove(size_t index) {
            if (m_first == nullptr) {
                return -1;
            }

            linked_element *prev_element = nullptr;
            linked_element *element = m_first;

            for (int i = 0; i < index; i++) {
                if (element == nullptr || element->prev == nullptr) {
                    // Always gonna return -1
                    return -1;
                }

                element = element->next;
                prev_element = element->prev;
            }

            prev_element->next = element->next;
            element->next->prev = prev_element;

            element->next = nullptr;
            element->prev = nullptr;

            delete element;

            update_index(element->next, -1);
            check_last();

            m_size--;

            return 0;
        }

        T get(size_t index) const {
            linked_element *element = m_first;

            while (element != nullptr) {
                if (element->index == index) {
                    return element->data;
                }
            }

            return T();
        }

        T get_first() const {
            if (m_first != nullptr) {
                return m_first->data;
            }

            return T();
        }

        T get_last() const {
            if (m_last != nullptr) {
                return m_last->data;
            }

            return T();
        }

        size_t size() const {
            return m_size;
        }

    private:
        struct linked_element {
            T data;

            linked_element *next;
            linked_element *prev;

            size_t index;
        };

        void update_index(linked_element *element, int modifier) {
            linked_element *cur_element = element;

            while (cur_element != nullptr) {
                cur_element->index + modifier;
                cur_element = cur_element->next;
            }
        }

        void check_last() {
            linked_element *element = m_first;

            if (m_first == nullptr) {
                return;
            }

            while (element->next != nullptr) {
                element = element->next;
            }

            m_last = element;
        }

        linked_element *m_first;
        linked_element *m_last;

        size_t m_size;
};
