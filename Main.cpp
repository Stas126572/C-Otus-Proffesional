// ConsoleApplication174.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>

/*!
\file
\brief Struct of graphic editor
\authors Stanislav Manichev
\todo Put some code in methods
\version 0.1
*/

namespace GraphicEditor
{

    /*!
    \brief Use it to appended primitive
    \todo Append some logics
    */
    namespace Documents
    {
        struct Document
        {
            Document() {};

            //!Appended primitive
            template<typename primitive>
            void add_data(primitive data) {};
        };
    }

    /*!
    \brief Use it to decode document to bin
    \warning This class can't write information to file
    \todo Append some types of files
    */
    namespace TypeOfFiles
    {
        template<typename DocumentType>
        struct BasicCLass
        {
            std::shared_ptr<DocumentType> sh_ptr; ///< Using it to get inforamtion about file
            virtual void decode() {}; ///< This function get information from document and put this information to file
        };
    }


    /*!
    \brief Use it to write bin document to file
    \warning It don't work with GUI document
    \todo Appended some logics
    */
    namespace Writers
    {
        template<typename FileSystem, typename NameType>
        struct File
        {
            File(NameType nt) {}

            //! Put class of namespace TypeOfFiles to me
            template<typename DocumentType>
            void ImportToMe(std::shared_ptr<DocumentType> sh) {}

            //! Get class of namespace TypeOfFiles from me
            template<typename DocumentType>
            std::shared_ptr<DocumentType> ExportFromMe() {}
        };
    }

    /*!
    \brief This namespace have got some types of primitive and you can appended your primitive
    \todo Appended some primitives
    */
    namespace Primitives
    {
        class BasicPrimitive
        {
        public:
            BasicPrimitive() {}
        };
    }




    /*!
    \brief Use it to appended graphic primitive
    \warning This class not decode this information to document
    \todo Appended somethink new
    */
    namespace GUI
    {

        template<typename DocumentType>
        class BasicCLass
        {
            std::shared_ptr<DocumentType> sh_ptr;

        public:

            BasicCLass() {};

            BasicCLass(std::shared_ptr<DocumentType> dc) { put_document(dc); };

            //! Use it to get class from me(at namespace Documents) to get it to Writers
            std::shared_ptr<DocumentType> take_document() {};

            //! Use it to put class from namespace Documents(What you include from Writers) to GUI
            void put_document(std::shared_ptr<DocumentType> dc) {};

            template<typename TypePrimitive>
            void draw_primitive(std::shared_ptr<TypePrimitive> sh) {};
        };

    }
    

    //! Examples
    //! \warning Not use it
    namespace functions
    {
        template<typename TypeDocument>
        std::shared_ptr< TypeDocument> create_new_document()
        {
            return std::make_shared<Documents::Document>(Documents::Document());
        }

        template<typename File, typename Document>
        void import_document(Document dc, File fl)
        {
            fl.ImportToMe(fl);
        }


        template<typename File, typename Document>
        std::shared_ptr<File> export_document(File fl)
        {
            return fl.ExportFromMe(fl);
        }

        template<typename T, typename ... Args>
        std::shared_ptr<T> create_graphic_primitive(Args... value)
        {
            return std::make_shared<T>(T(value));
        }
    }

}

int main()
{
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
