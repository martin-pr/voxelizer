#include <fstream>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>

#include "array_maths.h"
#include "factory.tpp"
#include "mesh.h"

#include <Alembic/Abc/All.h>
#include <Alembic/AbcGeom/All.h>
#include <Alembic/AbcCoreHDF5/All.h>
#include <Alembic/Abc/ITypedArrayProperty.h>

namespace {

using std::cout;
using std::endl;
using namespace Alembic::AbcGeom;

void parse(IObject iObj, mesh& result, Alembic::AbcGeom::M44d tr = Alembic::AbcGeom::M44d(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1)) {
	// get the object's metadata
	const MetaData &md = iObj.getMetaData();

	// accumulate the transformation
	if(IXform::matches(md)) {
		IXform x( iObj, kWrapExisting );
		XformSample xs;
		x.getSchema().get( xs );
		tr *= xs.getMatrix();
	}

	// the offset of vertex indices (adding vertices to the back of current mesh)
	const unsigned voffs = result.vertices().size();

	Abc::P3fArraySamplePtr positions;
	Abc::Int32ArraySamplePtr faceIndices;
	Abc::Int32ArraySamplePtr faceCounts;

	// extract the right kind of data
	if(IPolyMeshSchema::matches(md)) {
		IPolyMesh sd(iObj, kWrapExisting);
		IPolyMeshSchema::Sample sds;
		sd.getSchema().get(sds);

		// get the data
		positions = sds.getPositions();
		faceCounts = sds.getFaceCounts();
		faceIndices = sds.getFaceIndices();
	}

	else if(ISubDSchema::matches(md)) {
		ISubD sd(iObj, kWrapExisting);
		ISubDSchema::Sample sds;
		sd.getSchema().get(sds);

		// get the data
		positions = sds.getPositions();
		faceCounts = sds.getFaceCounts();
		faceIndices = sds.getFaceIndices();
	}

	// mesh parsing
	if(positions != NULL) {
		assert(faceIndices != NULL);
		assert(faceCounts != NULL);

		// copy all vertices
		for(size_t i = 0; i < positions->size(); ++i) {
			const auto& v = (*positions)[i] * tr;
			result.add_vertex({v.x, v.y, v.z});
		}

		// and perforrm trivial triangulation to assemble triangles
		size_t fii = 0;
		for(size_t fci = 0; fci < faceCounts->size(); ++fci) {
			assert(fii < faceIndices->size());

			const auto fc = (*faceCounts)[fci];
			assert(fc >= 3);
			for(unsigned c=0; c<fc-2u; ++c)
				result.add_face(std::array<unsigned, 3>({
					(*faceIndices)[fii] + voffs,
					(*faceIndices)[fii + c + 1] + voffs,
					(*faceIndices)[fii + c + 2] + voffs,
				}));

			fii += fc;
		}
	}

	// now the child objects
	for(size_t i = 0; i < iObj.getNumChildren(); i++ )
		parse(IObject(iObj, iObj.getChildHeader(i).getName()), result, tr);
}

factory<mesh> s_abcFactory(".abc", [](boost::filesystem::path p) {
	std::unique_ptr<mesh> result(new mesh());

	IArchive archive(Alembic::AbcCoreHDF5::ReadArchive(), p.string());

	parse(archive.getTop(), *result);

	return std::move(result);
});

}
